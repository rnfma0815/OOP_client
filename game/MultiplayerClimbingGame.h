#pragma once

#include <string>
#include <vector>

class MultiplayerClimbingGame {
private:
    struct RoomInfo {
        std::string code;
        std::string name;
        int currentPlayer;
        int maxPlayer;
        bool playing;
        std::string playerNames;
    };

    std::vector<std::string> menuItems;
    std::string serverHost;
    unsigned short serverPort;

public:
    MultiplayerClimbingGame();
    void run();
    void createRoom();
    void showRoomList();
    void showDescription() const;

private:
    void drawMenu(int selected) const;
    void drawRoomList(const std::vector<RoomInfo>& rooms, int selected, const std::string& status) const;
    void drawWaitingRoom(const RoomInfo& room, const std::string& status) const;
    void joinRoomByCode();
    void joinRoom(const std::string& roomCode, const std::string& roomName);
    void waitForGameStart(class NetworkClient& client, const std::string& nickname) const;
    void waitInCreatedRoom(class NetworkClient& client, const RoomInfo& room, const std::string& nickname) const;
    void playMatch(class NetworkClient& client, const std::string& nickname, const std::string& p1Name, const std::string& p2Name, int roundNumber, int maxRounds, int setNumber, int maxSets) const;

    std::vector<RoomInfo> parseRoomList(const std::string& message) const;
    bool fetchRoomList(std::vector<RoomInfo>& rooms, std::string& status) const;
    std::vector<std::string> split(const std::string& text, char delimiter) const;
    std::string readText(const std::string& label, const std::string& defaultValue) const;
    std::string makeRoomSummaryText(int number, const RoomInfo& room, bool selected) const;
    std::string selectCard(const std::vector<std::string>& availableCards,
        const std::string& localName,
        const std::string& opponentName,
        int roundNumber,
        int maxRounds,
        int setNumber,
        int maxSets,
        int p1RoundScore,
        int p2RoundScore,
        int p1SetWins,
        int p2SetWins,
        bool& cancelled) const;
    void showDisconnectedMessage(const std::string& playerName) const;
    std::string protocolCardToDisplayName(const std::string& card) const;
    int getDisplayWidth(const std::string& text) const;
    std::string makePadding(const std::string& text, int targetWidth) const;
};
