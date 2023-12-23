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
#include "ConnectedApp.h"
#include <atlbase.h>
#import "sldworks.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "swconst.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids



class ConnectedSolidworks :
    public ConnectedApp
{
    CComPtr<ISldWorks> swApp;   // Pointer to SolidWorks application
    CComPtr<IModelView> activeView;  // Active view in SolidWorks
	bool ComInstanceCreated = false;
public:
	ConnectedSolidworks() : ConnectedApp("SolidWorks") {}
	// Function called when the user requests to connect to an external application
	bool onConnect();
	// Function called every ~20ms, this is where HID data
	void timerCall(double data[6]);
	// Function called when the user requests to disconnect from an external application
	void onDisconnect();
	~ConnectedSolidworks() {}
};

