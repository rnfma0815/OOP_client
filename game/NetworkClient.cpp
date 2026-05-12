#include "NetworkClient.h"

#include <ws2tcpip.h>

NetworkClient::NetworkClient()
    : socketHandle(INVALID_SOCKET), wsaStarted(false) {
}

NetworkClient::~NetworkClient() {
    disconnect();

    if (wsaStarted) {
        WSACleanup();
        wsaStarted = false;
    }
}

bool NetworkClient::connectToServer(const std::string& host, unsigned short port) {
    disconnect();

    if (!startWinsock()) {
        return false;
    }

    socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketHandle == INVALID_SOCKET) {
        return false;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &serverAddress.sin_addr) != 1) {
        disconnect();
        return false;
    }

    if (connect(socketHandle, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        disconnect();
        return false;
    }

    return true;
}

void NetworkClient::disconnect() {
    receiveBuffer.clear();

    if (socketHandle != INVALID_SOCKET) {
        shutdown(socketHandle, SD_BOTH);
        closesocket(socketHandle);
        socketHandle = INVALID_SOCKET;
    }
}

bool NetworkClient::sendMessage(const std::string& message) {
    if (!isConnected()) {
        return false;
    }

    std::string line = message + "\n";
    const char* data = line.c_str();
    int remaining = static_cast<int>(line.size());

    while (remaining > 0) {
        int sent = send(socketHandle, data, remaining, 0);
        if (sent == SOCKET_ERROR) {
            disconnect();
            return false;
        }

        data += sent;
        remaining -= sent;
    }

    return true;
}

bool NetworkClient::receiveLine(std::string& line) {
    line.clear();

    while (true) {
        size_t newlinePos = receiveBuffer.find('\n');
        if (newlinePos != std::string::npos) {
            line = receiveBuffer.substr(0, newlinePos);
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            receiveBuffer.erase(0, newlinePos + 1);
            return true;
        }

        char buffer[512];
        int received = recv(socketHandle, buffer, sizeof(buffer), 0);
        if (received <= 0) {
            disconnect();
            return false;
        }

        receiveBuffer.append(buffer, received);
    }
}

bool NetworkClient::receiveLineWithTimeout(std::string& line, int timeoutMs) {
    line.clear();

    size_t newlinePos = receiveBuffer.find('\n');
    if (newlinePos != std::string::npos) {
        line = receiveBuffer.substr(0, newlinePos);
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        receiveBuffer.erase(0, newlinePos + 1);
        return true;
    }

    if (!isConnected()) {
        return false;
    }

    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(socketHandle, &readSet);

    timeval timeout;
    timeout.tv_sec = timeoutMs / 1000;
    timeout.tv_usec = (timeoutMs % 1000) * 1000;

    int ready = select(0, &readSet, nullptr, nullptr, &timeout);
    if (ready <= 0) {
        return false;
    }

    char buffer[512];
    int received = recv(socketHandle, buffer, sizeof(buffer), 0);
    if (received <= 0) {
        disconnect();
        return false;
    }

    receiveBuffer.append(buffer, received);

    newlinePos = receiveBuffer.find('\n');
    if (newlinePos == std::string::npos) {
        return false;
    }

    line = receiveBuffer.substr(0, newlinePos);
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }

    receiveBuffer.erase(0, newlinePos + 1);
    return true;
}

bool NetworkClient::isConnected() const {
    return socketHandle != INVALID_SOCKET;
}

bool NetworkClient::startWinsock() {
    if (wsaStarted) {
        return true;
    }

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        return false;
    }

    wsaStarted = true;
    return true;
}
