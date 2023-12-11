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
	ConnectedSolidworks(std::string appName) : ConnectedApp(appName) {}
	// Function called when the user requests to connect to an external application
	bool onConnect();
	// Function called every ~20ms, this is where HID data
	void timerCall(double data[6]);
	// Function called when the user requests to disconnect from an external application
	void onDisconnect();
	~ConnectedSolidworks() {}
};

