#include "ConnectedSolidworks.h"
#include "Utilities.h"


bool ConnectedSolidworks::onConnect() {
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        wxMessageBox("Failed to initialize COM", "Error", wxICON_ERROR);
        return false;
    }

    if (!ComInstanceCreated) {
        // Create SolidWorks instance
        if (swApp.CoCreateInstance(__uuidof(SldWorks), NULL, CLSCTX_LOCAL_SERVER) != S_OK) {
            wxMessageBox("Failed to create SolidWorks instance", "Error", wxICON_ERROR);
            CoUninitialize();
            return false;
        }
        ComInstanceCreated = true;
    }

    // Make SolidWorks visible
    swApp->put_Visible(VARIANT_TRUE);

    // Get the active document and view
    CComPtr<IModelDoc2> activeDoc;
    hr = swApp->get_IActiveDoc2(&activeDoc);
    if (FAILED(hr) || !activeDoc) {
        wxMessageBox("Failed to get active document", "Error", wxICON_ERROR);
        return false;
    }

    hr = activeDoc->get_IActiveView(&activeView);
    if (FAILED(hr) || !activeView) {
        wxMessageBox("Failed to get active view", "Error", wxICON_ERROR);
        return false;
    }

    return true;
}

void ConnectedSolidworks::timerCall(double data[6]) {

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

void ConnectedSolidworks::onDisconnect() {

    // Clear the CComPtr
    activeView.Release();
    // Uninitialize COM
    CoUninitialize();
}

