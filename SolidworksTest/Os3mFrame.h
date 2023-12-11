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

