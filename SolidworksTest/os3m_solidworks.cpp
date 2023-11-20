/*
Copyright(C) 2023 Colton Baldridge

This program is free software : you can redistribute it and /or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see < https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Windows.h>
#include <atlbase.h>
#include "hidapi.h"

#import "sldworks.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "swconst.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids

#define TRANSLATE_X_MULTIPLIER 0.00004  // Shifting left/right on the screen
#define TRANSLATE_Y_MULTIPLIER 0.00020  // Zoom
#define TRANSLATE_Z_MULTIPLIER 0.000007  // Shifting up/down on the screen
#define ROTATE_X_MULTIPLIER 0.00009  // Roll about x-axis
#define ROTATE_Y_MULTIPLIER 0.00006  // Roll about y-axis
#define ROTATE_Z_MULTIPLIER 0.0002  // Roll about z-axis

// Function to read USB HID data
void get_hid_data(double data[6]) {
    // Initialize the hidapi library
    hid_init();

    // Open the device using the VID, PID
    hid_device* handle = hid_open(0x0483, 0x572b, NULL);
    if (!handle) {
        std::cerr << "Cannot open HID device" << std::endl;
        return;
    }

    unsigned char buf[256];
    int res;

    // Read requested state
    res = hid_read(handle, buf, sizeof(buf));
    if (res < 0) {
        std::cerr << "Read error" << std::endl;
    }
    else {
        // Parse the data to doubles
        int16_t tmp[6];
        std::memcpy(tmp, buf, sizeof(tmp));
        for (int i = 0; i < 6; i++) {
            // Converting int16_t to double
            data[i] = static_cast<double>(tmp[i]);
        }

        // data[0] = X (left/right translate on screen)
        // data[1] = Y (zoom)
        // data[2] = Z (up/down translate on screen)
        // data[3] = RX (roll about x-axis)
        // data[4] = RY (roll about y-axis)
        // data[5] = RZ (roll about z-axis)
    }

    // Close the device
    hid_close(handle);

    // Finalize the hidapi library
    hid_exit();
}

// Function to check if all elements of an array are zero
bool checkZeroes(double data[6]) {
    bool allZeroes = true;

    for (int i = 0; i < 6; i++) {
        allZeroes = allZeroes && data[i] == 0.0;
    }

    return allZeroes;
}

int main() {
    // Initialize COM
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize COM" << std::endl;
        return -1;
    }

    {
        CComPtr<ISldWorks> swApp;

        // Create SolidWorks instance
        if (swApp.CoCreateInstance(__uuidof(SldWorks), NULL, CLSCTX_LOCAL_SERVER) != S_OK) {
            std::cerr << "Failed to create SolidWorks instance" << std::endl;
            CoUninitialize();
            return -1;
        }

        // Make SolidWorks visible
        swApp->put_Visible(VARIANT_TRUE);

        // Initialize variables
        CComPtr<IModelDoc2> activeDoc;
        CComPtr<IModelView> activeView;

        // Get active document
        hr = swApp->get_IActiveDoc2(&activeDoc);
        if (FAILED(hr) || !activeDoc) {
            std::cerr << "Failed to get active document" << std::endl;
        }

        // Get active view
        hr = activeDoc->get_IActiveView(&activeView);
        if (FAILED(hr) || !activeView) {
            std::cerr << "Failed to get active view" << std::endl;
        }

        // Continuously read and process data from the HID device
        while(1){

            // Parse data and store it in an array
            double data[6] = {0};
            get_hid_data(data);

            // Filter out small values
            for (int i = 0; i < 6; i++) {
                //if (abs(data[i]) < 10) {
                //   data[i] = 0;
                //}
                std::printf("%6.1f ", data[i]);
            }
            std::cout << std::endl;


            // Update the camera position and orientation if data is not all zeroes
            if (!checkZeroes(data)) {
                activeView->put_EnableGraphicsUpdate(false);
                 
                if (data[0] != 0.0 || data[2] != 0.0) {
                    activeView->TranslateBy(data[0] * TRANSLATE_X_MULTIPLIER, data[2] * TRANSLATE_Z_MULTIPLIER);
                }
                if (data[1] != 0.0) {
                    activeView->ZoomByFactor((1.0 + data[1] * TRANSLATE_Y_MULTIPLIER));
                }
                if (data[3] != 0.0 || data[5] != 0.0) {
                    activeView->RotateAboutCenter(data[3] * ROTATE_X_MULTIPLIER, -data[5] * ROTATE_Z_MULTIPLIER);
                }
                if (data[4] != 0.0) {
                    activeView->RollBy(data[4] * ROTATE_Y_MULTIPLIER);
                }
                activeView->put_EnableGraphicsUpdate(true);
            }
        }
    }

    // Uninitialize COM
    CoUninitialize();
    return 0;
}
