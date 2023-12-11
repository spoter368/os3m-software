#pragma once
#include <wx/wx.h>
#include <atlbase.h>
#include "hidapi.h"
#include "ConnectedApp.h"
#import "sldworks.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "swconst.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids

class Os3mFrame: public wxFrame
{
private:
    wxTextCtrl* dataText[6];  // Array of text controls for data display
    wxTimer* updateTimer;        // Timer for periodic updates
    wxButton* connectButton;  // Button for connecting to apps
    wxListBox* appListBox;  // List box for displaying app names
    wxStaticText* connectionStatusText;  // Text control for displaying connection status
    hid_device* handle;  // Pointer the the mouse's HID device
    ConnectedApp* currentApp;  // Pointer to the currently connected app
    std::vector<ConnectedApp*> appList;  // List of apps you can connect to

public:
    Os3mFrame(const wxString& title);
    ~Os3mFrame();
    void MouseConnect();
    void OnUpdateTimer(wxTimerEvent& event);
    void OnConnectButtonClicked(wxCommandEvent& event);
    void PopulateAppList();
    void OnAppSelected(wxCommandEvent& event);  // Event handler for list box selection

};

