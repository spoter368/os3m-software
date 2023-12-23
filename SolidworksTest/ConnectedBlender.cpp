#include "ConnectedBlender.h"
#include <iostream>
#include <sstream>

#pragma comment(lib,"ws2_32.lib")

ConnectedBlender::ConnectedBlender() : ConnectedApp("Blender"), sock(INVALID_SOCKET) {
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Failed to initialize Winsock. Error Code : " << WSAGetLastError() << std::endl;
    }
}

bool ConnectedBlender::onConnect() {
    const std::string address = "127.0.0.1"; // Server IP
    int port = 36800; // Server Port, chosen randomly

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Could not create socket : " << WSAGetLastError() << std::endl;
        return false;
    }

    server.sin_addr.s_addr = inet_addr(address.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Connect to remote server
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Connect error : " << WSAGetLastError() << std::endl;
        return false;
    }

    connected = true;
    return true;
}

void ConnectedBlender::timerCall(double data[6]) {
    std::stringstream ss;
    for (int i = 0; i < 6; ++i) {
        ss << data[i];
        if (i < 5) ss << ",";
    }

    std::string message = ss.str();
    if (send(sock, message.c_str(), message.length(), 0) < 0) {
        connected = false;
    }
}

void ConnectedBlender::onDisconnect() {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
    connected = false;
}

ConnectedBlender::~ConnectedBlender() {
    if (connected) {
        onDisconnect();
    }
    WSACleanup();
}
