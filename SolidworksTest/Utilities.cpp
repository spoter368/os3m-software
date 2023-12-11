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

#include "Utilities.h"
#include "hidapi.h"
#include <iostream>
#include <cstdint>

// Function to read USB HID data
void get_hid_data(double data[6], hid_device* handle) {


    unsigned char buf[13] = { 0 };
    int res = 0;

    // Flush the buffer by reading all the data
    while (hid_read(handle, buf, sizeof(buf)) > 0) {}


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

// Function to check if all elements of an array are zero
bool checkZeroes(double data[6]) {
    bool allZeroes = true;

    for (int i = 0; i < 6; i++) {
        allZeroes = allZeroes && data[i] == 0.0;
    }

    return allZeroes;
}