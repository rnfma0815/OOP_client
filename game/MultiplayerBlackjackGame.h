#pragma once

#include <string>
#include <vector>
#include "Card.h"

class NetworkClient;

class MultiplayerBlackjackGame {
private:
    struct RoomInfo {
        std::string code;
        std::string name;
        int currentPlayer;
        int maxPlayer;
        bool playing;
        std::string playerNames;
        std::string gameType;
    };

    std::string serverHost;
    unsigned short serverPort;
    std::vector<std::string> menuItems;

public:
    MultiplayerBlackjackGame();
    void run();
    void createRoom();
    void showRoomList();
    void showDescription() const;

private:
    void drawMenu(int selected) const;
    void drawRoomList(const std::vector<RoomInfo>& rooms, int selected, const std::string& status) const;
    void drawWaitingRoom(const std::string& roomCode, const std::string& roomName, const std::string& nickname, const std::string& status) const;
    void joinRoom(const std::string& roomCode, const std::string& roomName);
    void waitForGameStart(NetworkClient& client, const std::string& nickname) const;
    void playMatch(NetworkClient& client, const std::string& nickname, const std::string& firstState) const;
    void showDisconnectedMessage(const std::string& playerName) const;

    bool fetchRoomList(std::vector<RoomInfo>& rooms, std::string& status) const;
    std::vector<RoomInfo> parseRoomList(const std::string& message) const;
    std::vector<Card> parseCards(const std::string& text) const;
    Card parseCard(const std::string& text) const;
    std::vector<std::string> split(const std::string& text, char delimiter) const;
    std::string readText(const std::string& label, const std::string& defaultValue) const;
    int getDisplayWidth(const std::string& text) const;
    std::string makePadding(const std::string& text, int targetWidth) const;
};
