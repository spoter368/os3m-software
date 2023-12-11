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

#include "Os3mFrame.h"
#include <wx/wx.h>
#include "Utilities.h"
#include "hidapi.h"
#include "ConnectedSolidworks.h"

Os3mFrame::Os3mFrame(const wxString& title): wxFrame(nullptr, wxID_ANY, title)
{
    // Here is where you add all your ConnectedApps!
    appList.push_back(new ConnectedSolidworks("SolidWorks"));

    // Timer for API calls
    updateTimer = new wxTimer();
    // Set up the update timer
    updateTimer->SetOwner(this);
    //updateTimer->Bind(wxEVT_TIMER, &MyFrame::OnUpdateTimer, this); // why doesn't this work??
    Bind(wxEVT_TIMER, &Os3mFrame::OnUpdateTimer, this, wxID_ANY);
    updateTimer->Start(20); // Update every 20 ms (50Hz)

    // Button user clicks to engage a connection
    connectButton = new wxButton(this, wxID_ANY, wxT("Connect"), wxPoint(60,160), wxSize(90, 24));
    connectButton->Bind(wxEVT_BUTTON, &Os3mFrame::OnConnectButtonClicked, this);

    // Status text for telling the user we successfully connected
    connectionStatusText = new wxStaticText(this, wxID_ANY, "", wxPoint(160, 164), wxDefaultSize, wxALIGN_CENTRE);

    // Labels for the data
    wxString labels[] = { "X", "Y", "Z", "Rx", "Ry", "Rz" };

    // Initialize data displays
    for (int i = 0; i < 6; ++i) {

        // Create the label
        new wxStaticText(this, wxID_ANY, labels[i], wxPoint(240 + i / 3 * 70 - 17, 13 + i % 3 * 20), wxSize(20, 20));

        // Create wxTextCtrl to hold the data
        dataText[i] = new wxTextCtrl(this, wxID_ANY, "0.0", wxPoint(240 + i / 3 * 70, 10 + i % 3 * 20), wxSize(50, 20), wxTE_READONLY);

        // Set color and font
        dataText[i]->SetBackgroundColour(wxColour(255, 255, 255));
        dataText[i]->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        dataText[i]->Refresh();
    }

    // Initialize the hidapi library
    hid_init();

    // Connect to mouse
    MouseConnect();

    // Create the title for the app list box
    wxStaticText* appListTitle = new wxStaticText(this, wxID_ANY, wxT("Supported Apps"), wxPoint(30, 10), wxDefaultSize);

    // Set the font of the title to be bold
    wxFont titleFont = appListTitle->GetFont();
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    appListTitle->SetFont(titleFont);

    // Set up the list of connectable apps
    appListBox = new wxListBox(this, wxID_ANY, wxPoint(30, 30), wxSize(150, 120));
    appListBox->Bind(wxEVT_LISTBOX, &Os3mFrame::OnAppSelected, this);
    PopulateAppList();
}

Os3mFrame::~Os3mFrame()
{
    // Close the HID device
    hid_close(handle);

    // Finalize the hidapi library
    hid_exit();

    delete(updateTimer);

    for (ConnectedApp* c : appList) {
        delete c;
    }
}


void Os3mFrame::MouseConnect()
{
    // Open the device using the VID, PID
    handle = hid_open(0x0483, 0x572b, NULL);
    if (!handle) {
        wxMessageBox("Cannot connect to mouse's HID device", "Error", wxICON_ERROR);
    }

    // Make sure that HID reads are nonblocking, so we can clear the HID buffer in get_hid_data()
    hid_set_nonblocking(handle, 1);
}

void Os3mFrame::OnUpdateTimer(wxTimerEvent& event)
{
    double data[6] = { 0 };
    get_hid_data(data, handle);

    for (int i = 0; i < 6; ++i) {
        dataText[i]->SetLabel(wxString::Format("%5.0f", data[i]));
    }

    if (currentApp != nullptr) {
        if (currentApp->connected) {
            currentApp->timerCall(data);
        }
    }

}

void Os3mFrame::OnConnectButtonClicked(wxCommandEvent& event)
{
    if (currentApp != nullptr) {
        // Handle connection and disconnection from apps
        if (connectButton->GetLabel() == "Connect") {
            bool connectSuccess = currentApp->onConnect();
            if (connectSuccess) {
                currentApp->connected = true;
                appListBox->Enable(false);
                wxString statusMessage = wxString::Format("Connection to %s successful!", currentApp->name);
                connectionStatusText->SetLabel(statusMessage);
            }
            connectButton->SetLabel("Disconnect");
        }
        else {
            currentApp->onDisconnect();
            currentApp->connected = false;
            connectionStatusText->SetLabel("");
            appListBox->Enable(true);
            connectButton->SetLabel("Connect");
        }
    }
}

void Os3mFrame::PopulateAppList()
{
    appListBox->Clear();  // Clear existing items
    for (ConnectedApp* app : appList) {
        appListBox->Append(app->name);  // Add each app name to the list box
    }
}

void Os3mFrame::OnAppSelected(wxCommandEvent& event)
{
    int selection = appListBox->GetSelection();
    if (selection != wxNOT_FOUND) {
        currentApp = appList[selection];
    }
}
