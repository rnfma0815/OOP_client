#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>

#include <string>

class NetworkClient {
private:
    SOCKET socketHandle;
    bool wsaStarted;
    std::string receiveBuffer;

public:
    NetworkClient();
    ~NetworkClient();

    NetworkClient(const NetworkClient&) = delete;
    NetworkClient& operator=(const NetworkClient&) = delete;

    bool connectToServer(const std::string& host, unsigned short port);
    void disconnect();
    bool sendMessage(const std::string& message);
    bool receiveLine(std::string& line);
    bool receiveLineWithTimeout(std::string& line, int timeoutMs);
    bool isConnected() const;

private:
    bool startWinsock();
};
