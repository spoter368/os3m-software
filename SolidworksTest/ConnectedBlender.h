#pragma once


#include "ConnectedApp.h"
#include <winsock2.h>
#include <string>

class ConnectedBlender : public ConnectedApp {
private:
    SOCKET sock;
    struct sockaddr_in server;
    WSADATA wsa;

public:
    ConnectedBlender();
    virtual bool onConnect() override;
    virtual void timerCall(double data[6]) override;
    virtual void onDisconnect() override;
    virtual ~ConnectedBlender();
};


