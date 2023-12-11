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
#include <string>



// Base class for all external applications the OS3M mouse will connect to
class ConnectedApp {

public:
	// Name of the program to show in the list
	const std::string name;
	// Whether the app is currently connected to
	bool connected = false;
	// Constructor
	ConnectedApp(std::string appName) : name(appName){}
	// Function called when the user requests to connect to an external application
	virtual bool onConnect() = 0;
	// Function called every ~20ms, this is where HID data is interpreted
	// data[0] = X (left/right translate on screen)
	// data[1] = Y (zoom)
	// data[2] = Z (up/down translate on screen)
	// data[3] = RX (roll about x-axis)
	// data[4] = RY (roll about y-axis)
	// data[5] = RZ (roll about z-axis)
	virtual void timerCall(double data[6]) = 0;
	// Function called when the user requests to disconnect from an external application
	virtual void onDisconnect() = 0;
	virtual ~ConnectedApp() {}
};

