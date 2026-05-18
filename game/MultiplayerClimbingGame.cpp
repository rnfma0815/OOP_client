#include "MultiplayerClimbingGame.h"
#include "ClimbingGameView.h"
#include "NetworkClient.h"
#include "PauseMenuView.h"
#include "../common/ProtocolUtil.h"

#include <algorithm>
#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <sstream>

MultiplayerClimbingGame::MultiplayerClimbingGame()
    : serverHost("127.0.0.1"), serverPort(9000) {
    menuItems.push_back("Create Room");
    menuItems.push_back("Room List");
    menuItems.push_back("Description");
    menuItems.push_back("Back");
}

void MultiplayerClimbingGame::run() {
    int selected = 0;

    while (true) {
        drawMenu(selected);

        int key = _getch();

        if (key == 224 || key == 0) {
            int arrow = _getch();

            if (arrow == 72) {
                selected = (selected - 1 + (int)menuItems.size()) % (int)menuItems.size();
            }
            else if (arrow == 80) {
                selected = (selected + 1) % (int)menuItems.size();
            }
        }
        else if (key == 13) {
            if (selected == 0) {
                createRoom();
            }
            else if (selected == 1) {
                showRoomList();
            }
            else if (selected == 2) {
                showDescription();
            }
            else if (selected == 3) {
                break;
            }
        }
        else if (key == 27) {
            break;
        }
    }
}

void MultiplayerClimbingGame::drawMenu(int selected) const {
    system("cls");
    system("color 0E");

    std::cout << "========================================================\n";
    std::cout << "              ◀  Online Climbing Game  ▶\n";
    std::cout << "========================================================\n\n";

    std::cout << "       ┌────────┐  ┌────────┐  ┌────────┐  ┌────────┐\n";
    std::cout << "       │K       │  │Q       │  │J       │  │JOKER   │\n";
    std::cout << "       │        │  │        │  │        │  │        │\n";
    std::cout << "       │   ♠    │  │   ♥    │  │   ♦    │  │   ★    │\n";
    std::cout << "       │        │  │        │  │        │  │        │\n";
    std::cout << "       │       K│  │       Q│  │       J│  │   JOKER│\n";
    std::cout << "       └────────┘  └────────┘  └────────┘  └────────┘\n\n";

    for (int i = 0; i < (int)menuItems.size(); i++) {
        if (i == selected) {
            std::cout << "                     ▶ " << menuItems[i] << " ◀\n";
        }
        else {
            std::cout << "                       " << menuItems[i] << "\n";
        }
    }

    std::cout << "\n";
    std::cout << "        Use UP / DOWN to move menu\n";
    std::cout << "        Press ENTER to select\n";
}

void MultiplayerClimbingGame::drawRoomList(const std::vector<RoomInfo>& rooms, int selected, const std::string& status) const {
    system("cls");
    system("color 0E");

    std::cout << "====================== ROOM LIST ======================\n\n";

    const int listAreaHeight = 10;
    const int boxWidth = 58;
    int printedRows = 0;
    std::string horizontalLine;

    for (int i = 0; i < boxWidth; i++) {
        horizontalLine += "─";
    }

    std::cout << " ┌" << horizontalLine << "┐\n";

    if (rooms.empty()) {
        std::string text = "방이 비어있습니다.";
        std::cout << " │ " << text << makePadding(text, boxWidth - 1) << "│\n";
        printedRows++;
    }
    else {
        std::string headerText = "No.  Code        Room Name          Players  Status";
        std::cout << " │ " << headerText << makePadding(headerText, boxWidth - 1) << "│\n";
        std::cout << " ├" << horizontalLine << "┤\n";
        printedRows += 2;

        int maxVisibleRooms = listAreaHeight - printedRows;
        if (maxVisibleRooms < 1) {
            maxVisibleRooms = 1;
        }
        int firstRoom = 0;

        if (selected >= maxVisibleRooms) {
            firstRoom = selected - maxVisibleRooms + 1;
        }
        if (firstRoom + maxVisibleRooms > (int)rooms.size()) {
            firstRoom = (int)rooms.size() - maxVisibleRooms;
        }
        if (firstRoom < 0) {
            firstRoom = 0;
        }

        int lastRoom = firstRoom + maxVisibleRooms;
        if (lastRoom > (int)rooms.size()) {
            lastRoom = (int)rooms.size();
        }

        for (int i = firstRoom; i < lastRoom; i++) {
            const RoomInfo& room = rooms[i];
            std::string rowText = makeRoomSummaryText(i + 1, room, selected == i);

            std::cout << " │ " << rowText
                      << makePadding(rowText, boxWidth - 1)
                      << "│\n";
            printedRows++;
        }
    }

    while (printedRows < listAreaHeight) {
        std::cout << " │" << std::string(boxWidth, ' ') << "│\n";
        printedRows++;
    }

    std::cout << " └" << horizontalLine << "┘\n\n";

    int joinCodeIndex = (int)rooms.size();
    int refreshIndex = joinCodeIndex + 1;
    int backIndex = refreshIndex + 1;

    if (selected == joinCodeIndex) {
        std::cout << " ▶ Join by Code ◀\n";
    }
    else {
        std::cout << "   Join by Code\n";
    }

    if (selected == refreshIndex) {
        std::cout << " ▶ Refresh ◀\n";
    }
    else {
        std::cout << "   Refresh\n";
    }

    if (selected == backIndex) {
        std::cout << " ▶ Back ◀\n";
    }
    else {
        std::cout << "   Back\n";
    }

    std::cout << "\n";
    if (!status.empty()) {
        std::cout << " " << status << "\n";
    }
    std::cout << " Use UP / DOWN to move menu\n";
    std::cout << " Press ENTER to select, R to refresh\n";
}

void MultiplayerClimbingGame::drawWaitingRoom(const RoomInfo& room, const std::string& status) const {
    system("cls");
    system("color 0E");

    const int boxWidth = 58;
    std::string horizontalLine;

    for (int i = 0; i < boxWidth; i++) {
        horizontalLine += "─";
    }

    std::cout << "===================== WAITING ROOM =====================\n\n";
    std::cout << " ┌" << horizontalLine << "┐\n";

    std::string headerText = "No.  Code        Room Name          Players  Status";
    std::cout << " │ " << headerText << makePadding(headerText, boxWidth - 1) << "│\n";
    std::cout << " │" << std::string(boxWidth, ' ') << "│\n";

    std::string summaryText = makeRoomSummaryText(1, room, false);
    std::string playerText = room.playerNames.empty() ? "-" : room.playerNames;
    std::string waitingText = room.playing ? "Game already started" : "waiting...";

    std::cout << " │ " << summaryText << makePadding(summaryText, boxWidth - 1) << "│\n";
    std::cout << " ├" << horizontalLine << "┤\n";
    std::cout << " │ " << playerText << makePadding(playerText, boxWidth - 1) << "│\n";
    std::cout << " │ " << waitingText << makePadding(waitingText, boxWidth - 1) << "│\n";

    for (int i = 0; i < 4; i++) {
        std::cout << " │" << std::string(boxWidth, ' ') << "│\n";
    }

    std::cout << " └" << horizontalLine << "┘\n\n";
    std::cout << " ▶ Back ◀\n\n";

    if (!status.empty()) {
        std::cout << " " << status << "\n";
    }
    std::cout << " Press ENTER to leave room\n";
}

void MultiplayerClimbingGame::createRoom() {
    system("cls");
    system("color 0E");

    std::cout << "===================== CREATE ROOM =====================\n\n";

    std::string nickname = readText("Nickname", "Player1");
    if (nickname == "__CANCEL__") return;
    std::string roomName = readText("Room name", "Climbing Room");
    if (roomName == "__CANCEL__") return;

    NetworkClient client;
    if (!client.connectToServer(serverHost, serverPort)) {
        std::cout << "\nFailed to connect to server. Run Server.exe first.\n";
        std::cout << "\nPress any key to return...";
        _getch();
        return;
    }

    client.sendMessage("CREATE_ROOM|" + ProtocolUtil::encode(roomName) + "|" + ProtocolUtil::encode(nickname));

    std::string response;
    if (!client.receiveLine(response)) {
        std::cout << "\nDisconnected from server.\n";
        std::cout << "\nPress any key to return...";
        _getch();
        return;
    }

    if (response.rfind("ROOM_CREATED|", 0) != 0) {
        std::cout << "\nServer: " << response << "\n";
        std::cout << "\nPress any key to return...";
        _getch();
        return;
    }

    std::vector<std::string> fields = split(response, '|');
    RoomInfo room;
    room.code = fields.size() > 1 ? fields[1] : "";
    room.name = fields.size() > 2 ? ProtocolUtil::decode(fields[2]) : roomName;
    room.currentPlayer = 1;
    room.maxPlayer = 2;
    room.playing = false;
    room.playerNames = nickname;

    waitInCreatedRoom(client, room, nickname);
}

void MultiplayerClimbingGame::showRoomList() {
    std::vector<RoomInfo> rooms;
    std::string status;
    int selected = 0;

    fetchRoomList(rooms, status);

    while (true) {
        int itemCount = (int)rooms.size() + 3;
        if (selected >= itemCount) {
            selected = itemCount - 1;
        }

        drawRoomList(rooms, selected, status);

        int key = _getch();

        if (key == 224 || key == 0) {
            int arrow = _getch();

            if (arrow == 72) {
                selected = (selected - 1 + itemCount) % itemCount;
            }
            else if (arrow == 80) {
                selected = (selected + 1) % itemCount;
            }
        }
        else if (key == 'r' || key == 'R') {
            fetchRoomList(rooms, status);
            selected = 0;
        }
        else if (key == 13) {
            int joinCodeIndex = (int)rooms.size();
            int refreshIndex = joinCodeIndex + 1;
            int backIndex = refreshIndex + 1;

            if (selected == joinCodeIndex) {
                joinRoomByCode();
                fetchRoomList(rooms, status);
                selected = 0;
            }
            else if (selected == refreshIndex) {
                fetchRoomList(rooms, status);
                selected = 0;
            }
            else if (selected == backIndex) {
                break;
            }
            else if (selected >= 0 && selected < (int)rooms.size()) {
                const RoomInfo& selectedRoom = rooms[selected];

                if (selectedRoom.playing || selectedRoom.currentPlayer >= selectedRoom.maxPlayer) {
                    status = "This room is not available.";
                    continue;
                }

                joinRoom(selectedRoom.code, selectedRoom.name);
                fetchRoomList(rooms, status);
                selected = 0;
            }
        }
        else if (key == 27) {
            break;
        }
    }
}

void MultiplayerClimbingGame::joinRoomByCode() {
    system("cls");
    system("color 0E");

    std::cout << "==================== JOIN BY CODE ====================\n\n";

    std::string roomCode = readText("Room code", "");
    if (roomCode == "__CANCEL__" || roomCode.empty()) {
        return;
    }

    joinRoom(roomCode, "Unknown Room");
}

void MultiplayerClimbingGame::joinRoom(const std::string& roomCode, const std::string& roomName) {
    system("cls");
    system("color 0E");

    std::cout << "====================== JOIN ROOM ======================\n\n";
    std::cout << "Room: " << roomCode << " | " << roomName << "\n\n";

    std::string nickname = readText("Nickname", "Player2");
    if (nickname == "__CANCEL__") return;

    NetworkClient client;
    if (!client.connectToServer(serverHost, serverPort)) {
        std::cout << "\nFailed to connect to server. Run Server.exe first.\n";
        std::cout << "\nPress any key to return...";
        _getch();
        return;
    }

    client.sendMessage("JOIN_ROOM|" + ProtocolUtil::encode(roomCode) + "|" + ProtocolUtil::encode(nickname));

    std::string response;
    if (!client.receiveLine(response)) {
        std::cout << "\nDisconnected from server.\n";
        std::cout << "\nPress any key to return...";
        _getch();
        return;
    }

    std::cout << "\nServer: " << response << "\n";

    if (response.rfind("ROOM_JOINED|", 0) != 0) {
        std::cout << "\nPress any key to return...";
        _getch();
        return;
    }

    waitForGameStart(client, nickname);
}

void MultiplayerClimbingGame::showDescription() const {
    system("cls");
    system("color 0E");

    std::cout << "=============== ONLINE CLIMBING GAME DESCRIPTION ===============\n\n";
    std::cout << "- Create Room: Make a new multiplayer room.\n";
    std::cout << "- Room List: Check open rooms and join one.\n";
    std::cout << "- The game starts when two players are in the same room.\n";

    std::cout << "\n\nPress any key to return...";
    _getch();
}

void MultiplayerClimbingGame::waitForGameStart(NetworkClient& client, const std::string& nickname) const {
    std::string message;

    while (client.receiveLine(message)) {
        std::cout << "Server: " << message << "\n";

        if (message.rfind("GAME_START", 0) == 0) {
            std::vector<std::string> fields = split(message, '|');
            std::string p1Name = fields.size() > 1 ? ProtocolUtil::decode(fields[1]) : "Player1";
            std::string p2Name = fields.size() > 2 ? ProtocolUtil::decode(fields[2]) : "Player2";
            int roundNumber = fields.size() > 3 ? std::atoi(fields[3].c_str()) : 1;
            int maxRounds = fields.size() > 4 ? std::atoi(fields[4].c_str()) : 4;
            int setNumber = fields.size() > 5 ? std::atoi(fields[5].c_str()) : 1;
            int maxSets = fields.size() > 6 ? std::atoi(fields[6].c_str()) : 5;
            playMatch(client, nickname, p1Name, p2Name, roundNumber, maxRounds, setNumber, maxSets);
            return;
        }

        if (message.rfind("ERROR|", 0) == 0) {
            break;
        }

        if (message.rfind("PLAYER_LEFT|", 0) == 0 || message.rfind("GAME_ABORTED|", 0) == 0) {
            std::vector<std::string> fields = split(message, '|');
            showDisconnectedMessage(fields.size() > 1 ? ProtocolUtil::decode(fields[1]) : "");
            return;
        }
    }

    showDisconnectedMessage("");
}

void MultiplayerClimbingGame::playMatch(NetworkClient& client, const std::string& nickname, const std::string& p1Name, const std::string& p2Name, int roundNumber, int maxRounds, int setNumber, int maxSets) const {
    std::vector<std::string> availableCards = { "KING", "QUEEN", "SERVANT", "JOKER" };
    int p1RoundScore = 0;
    int p2RoundScore = 0;
    int p1SetWins = 0;
    int p2SetWins = 0;
    bool amP1Player = nickname == p1Name;
    std::string localNameForScore = amP1Player ? p1Name : p2Name;
    std::string opponentNameForScore = amP1Player ? p2Name : p1Name;

    while (client.isConnected()) {
        int localRoundScoreForScreen = amP1Player ? p1RoundScore : p2RoundScore;
        int opponentRoundScoreForScreen = amP1Player ? p2RoundScore : p1RoundScore;
        int localSetWinsForScreen = amP1Player ? p1SetWins : p2SetWins;
        int opponentSetWinsForScreen = amP1Player ? p2SetWins : p1SetWins;
        bool cancelled = false;

        std::string selectedCard = selectCard(
            availableCards,
            localNameForScore,
            opponentNameForScore,
            roundNumber,
            maxRounds,
            setNumber,
            maxSets,
            localRoundScoreForScreen,
            opponentRoundScoreForScreen,
            localSetWinsForScreen,
            opponentSetWinsForScreen,
            cancelled
        );
        if (cancelled) {
            client.sendMessage("LEAVE_ROOM");
            client.disconnect();
            return;
        }
        client.sendMessage("SELECT_CARD|" + selectedCard);

        std::vector<std::string> actionLogs = { "Waiting..." };
        std::vector<std::string> cardNames;
        for (const std::string& card : availableCards) {
            cardNames.push_back(protocolCardToDisplayName(card));
        }
        int selectedIndex = 0;
        for (int i = 0; i < (int)availableCards.size(); i++) {
            if (availableCards[i] == selectedCard) {
                selectedIndex = i;
                break;
            }
        }
        ClimbingGameView waitingView;
        waitingView.drawGameScreen(
            cardNames,
            selectedIndex,
            localRoundScoreForScreen,
            opponentRoundScoreForScreen,
            roundNumber,
            maxRounds,
            setNumber,
            maxSets,
            localSetWinsForScreen,
            opponentSetWinsForScreen,
            "Online",
            localNameForScore,
            opponentNameForScore,
            actionLogs
        );

        bool needNextTurnSignal = false;

        std::string message;
        while (client.receiveLine(message)) {
            if (message.rfind("CARD_SELECTED|", 0) == 0) {
                continue;
            }

            if (message.rfind("ROUND_RESULT|", 0) == 0) {
                std::vector<std::string> fields = split(message, '|');
                if (fields.size() < 17) {
                    std::cout << "\nInvalid round result.\n";
                    std::cout << "\nPress any key to return...";
                    _getch();
                    return;
                }

                std::string serverP1Name = ProtocolUtil::decode(fields[1]);
                std::string serverP1Card = fields[2];
                std::string serverP2Name = ProtocolUtil::decode(fields[3]);
                std::string serverP2Card = fields[4];
                std::string winnerName = ProtocolUtil::decode(fields[5]);
                p1RoundScore = std::atoi(fields[6].c_str());
                p2RoundScore = std::atoi(fields[7].c_str());
                roundNumber = std::atoi(fields[8].c_str());
                maxRounds = std::atoi(fields[9].c_str());
                setNumber = std::atoi(fields[10].c_str());
                maxSets = std::atoi(fields[11].c_str());
                p1SetWins = std::atoi(fields[12].c_str());
                p2SetWins = std::atoi(fields[13].c_str());
                bool matchComplete = fields[15] == "true";
                std::string finalWinner = fields[16] == "-" ? "-" : ProtocolUtil::decode(fields[16]);

                bool amP1 = nickname == serverP1Name || (!nickname.empty() && p1Name == nickname);
            std::string localName = amP1 ? serverP1Name : serverP2Name;
            std::string opponentName = amP1 ? serverP2Name : serverP1Name;
            std::string localCard = amP1 ? serverP1Card : serverP2Card;
            std::string opponentCard = amP1 ? serverP2Card : serverP1Card;
            int localRoundScore = amP1 ? p1RoundScore : p2RoundScore;
            int opponentRoundScore = amP1 ? p2RoundScore : p1RoundScore;
            int localSetWins = amP1 ? p1SetWins : p2SetWins;
            int opponentSetWins = amP1 ? p2SetWins : p1SetWins;

                availableCards.erase(
                    std::remove(availableCards.begin(), availableCards.end(), localCard),
                    availableCards.end()
                );

                int result = 0;
                if (winnerName == localName) {
                    result = 1;
                }
                else if (winnerName == opponentName) {
                    result = -1;
                }

                ClimbingGameView view;
                std::string localCardName = protocolCardToDisplayName(localCard);
                std::string opponentCardName = protocolCardToDisplayName(opponentCard);
                view.drawMultiplayerRoundResult(
                    localName,
                    opponentName,
                    localCardName,
                opponentCardName,
                view.getRoleArt(localCardName),
                view.getRoleArt(opponentCardName),
                result,
                roundNumber,
                maxRounds,
                setNumber,
                maxSets,
                localRoundScore,
                opponentRoundScore,
                localSetWins,
                opponentSetWins
            );

                if (matchComplete) {
                    system("cls");
                    system("color 0E");
                    std::cout << "====================== GAME END ======================\n\n";
                    std::cout << "Match Score : " << serverP1Name << " " << p1SetWins
                              << "  -  " << serverP2Name << " " << p2SetWins << "\n\n";
                    if (finalWinner == "DRAW") {
                        std::cout << "Final result is draw.\n";
                    }
                    else {
                        std::cout << finalWinner << " wins the match!\n";
                    }
                    std::cout << "\nPress any key to return...";
                    _getch();
                    return;
                }

                needNextTurnSignal = true;
                break;
            }

            if (message.rfind("ERROR|", 0) == 0) {
                std::cout << "\nServer: " << message << "\n";
                std::cout << "\nPress any key to return...";
                _getch();
                return;
            }

            if (message.rfind("PLAYER_LEFT|", 0) == 0 || message.rfind("GAME_ABORTED|", 0) == 0) {
                std::vector<std::string> fields = split(message, '|');
                showDisconnectedMessage(fields.size() > 1 ? ProtocolUtil::decode(fields[1]) : opponentNameForScore);
                return;
            }
        }

        if (!needNextTurnSignal) {
            showDisconnectedMessage(opponentNameForScore);
            return;
        }

        bool gotNextTurnSignal = false;
        while (client.receiveLine(message)) {
            if (message.rfind("NEXT_ROUND|", 0) == 0) {
                std::vector<std::string> fields = split(message, '|');
                roundNumber = fields.size() > 1 ? std::atoi(fields[1].c_str()) : roundNumber + 1;
                maxRounds = fields.size() > 2 ? std::atoi(fields[2].c_str()) : maxRounds;
                setNumber = fields.size() > 3 ? std::atoi(fields[3].c_str()) : setNumber;
                maxSets = fields.size() > 4 ? std::atoi(fields[4].c_str()) : maxSets;
                p1SetWins = fields.size() > 5 ? std::atoi(fields[5].c_str()) : p1SetWins;
                p2SetWins = fields.size() > 6 ? std::atoi(fields[6].c_str()) : p2SetWins;
                gotNextTurnSignal = true;
                break;
            }

            if (message.rfind("NEXT_SET|", 0) == 0) {
                std::vector<std::string> fields = split(message, '|');
                setNumber = fields.size() > 1 ? std::atoi(fields[1].c_str()) : setNumber + 1;
                maxSets = fields.size() > 2 ? std::atoi(fields[2].c_str()) : maxSets;
                roundNumber = fields.size() > 3 ? std::atoi(fields[3].c_str()) : 1;
                maxRounds = fields.size() > 4 ? std::atoi(fields[4].c_str()) : maxRounds;
                p1SetWins = fields.size() > 5 ? std::atoi(fields[5].c_str()) : p1SetWins;
                p2SetWins = fields.size() > 6 ? std::atoi(fields[6].c_str()) : p2SetWins;
                p1RoundScore = 0;
                p2RoundScore = 0;
                availableCards = { "KING", "QUEEN", "SERVANT", "JOKER" };
                gotNextTurnSignal = true;
                break;
            }

            if (message.rfind("GAME_END|", 0) == 0) {
                return;
            }

            if (message.rfind("PLAYER_LEFT|", 0) == 0 || message.rfind("GAME_ABORTED|", 0) == 0) {
                std::vector<std::string> fields = split(message, '|');
                showDisconnectedMessage(fields.size() > 1 ? ProtocolUtil::decode(fields[1]) : opponentNameForScore);
                return;
            }
        }

        if (!gotNextTurnSignal) {
            showDisconnectedMessage(opponentNameForScore);
            return;
        }
    }
}

void MultiplayerClimbingGame::waitInCreatedRoom(NetworkClient& client, const RoomInfo& room, const std::string& nickname) const {
    RoomInfo currentRoom = room;
    std::string status;

    drawWaitingRoom(currentRoom, status);

    while (client.isConnected()) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 13) {
                client.sendMessage("LEAVE_ROOM");
                client.disconnect();
                return;
            }
        }

        std::string message;
        if (client.receiveLineWithTimeout(message, 100)) {
            if (message.rfind("PLAYER_JOINED|", 0) == 0) {
                std::vector<std::string> fields = split(message, '|');
                std::string joinedName = fields.size() > 1 ? ProtocolUtil::decode(fields[1]) : "Player2";
                currentRoom.currentPlayer = 2;
                currentRoom.playerNames = nickname + "/" + joinedName;
                status = joinedName + " joined.";
                drawWaitingRoom(currentRoom, status);
                continue;
            }

            if (message.rfind("GAME_START", 0) == 0) {
                std::vector<std::string> fields = split(message, '|');
                std::string p1Name = fields.size() > 1 ? ProtocolUtil::decode(fields[1]) : nickname;
                std::string p2Name = fields.size() > 2 ? ProtocolUtil::decode(fields[2]) : "Player2";
                currentRoom.currentPlayer = 2;
                currentRoom.playing = true;
                drawWaitingRoom(currentRoom, "GAME_START");
                int roundNumber = fields.size() > 3 ? std::atoi(fields[3].c_str()) : 1;
                int maxRounds = fields.size() > 4 ? std::atoi(fields[4].c_str()) : 4;
                int setNumber = fields.size() > 5 ? std::atoi(fields[5].c_str()) : 1;
                int maxSets = fields.size() > 6 ? std::atoi(fields[6].c_str()) : 5;
                playMatch(client, nickname, p1Name, p2Name, roundNumber, maxRounds, setNumber, maxSets);
                return;
            }

            if (message.rfind("ERROR|", 0) == 0) {
                status = message;
                drawWaitingRoom(currentRoom, status);
                continue;
            }

            if (message.rfind("PLAYER_LEFT|", 0) == 0 || message.rfind("GAME_ABORTED|", 0) == 0) {
                std::vector<std::string> fields = split(message, '|');
                showDisconnectedMessage(fields.size() > 1 ? ProtocolUtil::decode(fields[1]) : "Player");
                return;
            }
        }
    }

    showDisconnectedMessage("");
}

std::vector<MultiplayerClimbingGame::RoomInfo> MultiplayerClimbingGame::parseRoomList(const std::string& message) const {
    std::vector<RoomInfo> rooms;

    const std::string prefix = "ROOM_LIST|";
    if (message.rfind(prefix, 0) != 0) {
        return rooms;
    }

    std::string payload = message.substr(prefix.size());
    if (payload.empty()) {
        return rooms;
    }

    std::vector<std::string> roomItems = split(payload, ';');
    for (const std::string& item : roomItems) {
        std::vector<std::string> fields = split(item, ',');
        if (fields.size() < 5) {
            continue;
        }

        RoomInfo room;
        room.code = fields[0];
        room.name = ProtocolUtil::decode(fields[1]);
        room.currentPlayer = std::atoi(fields[2].c_str());
        room.maxPlayer = std::atoi(fields[3].c_str());
        room.playing = fields[4] == "true";
        room.playerNames = fields.size() >= 6 ? ProtocolUtil::decode(fields[5]) : "";
        std::string gameType = fields.size() >= 7 ? fields[6] : "CLIMBING";

        if (gameType != "CLIMBING") {
            continue;
        }

        rooms.push_back(room);
    }

    return rooms;
}

bool MultiplayerClimbingGame::fetchRoomList(std::vector<RoomInfo>& rooms, std::string& status) const {
    rooms.clear();

    NetworkClient client;
    if (!client.connectToServer(serverHost, serverPort)) {
        status = "Failed to connect to server. Run Server.exe first.";
        return false;
    }

    client.sendMessage("GET_ROOM_LIST");

    std::string response;
    if (!client.receiveLine(response)) {
        status = "Disconnected from server.";
        return false;
    }

    rooms = parseRoomList(response);
    status = rooms.empty() ? "" : "Room list updated. Press R to refresh.";
    return true;
}

std::vector<std::string> MultiplayerClimbingGame::split(const std::string& text, char delimiter) const {
    return ProtocolUtil::split(text, delimiter);
}

std::string MultiplayerClimbingGame::readText(const std::string& label, const std::string& defaultValue) const {
    std::string value;

    if (defaultValue.empty()) {
        std::cout << label << ": ";
    }
    else {
        std::cout << label << " [" << defaultValue << "]: ";
    }

    while (true) {
        int key = _getch();

        if (key == 27) {
            return "__CANCEL__";
        }
        if (key == 13) {
            std::cout << "\n";
            break;
        }
        if (key == 8) {
            if (!value.empty()) {
                value.pop_back();
                std::cout << "\b \b";
            }
            continue;
        }
        if (key >= 32 && key <= 126) {
            value.push_back((char)key);
            std::cout << (char)key;
        }
    }

    if (value.empty()) {
        return defaultValue;
    }

    return value;
}

std::string MultiplayerClimbingGame::makeRoomSummaryText(int number, const RoomInfo& room, bool selected) const {
    std::string marker = selected ? ">" : " ";
    std::string noText = marker + " " + std::to_string(number);
    std::string codeText = room.code;
    std::string roomName = room.name;
    std::string playersText = std::to_string(room.currentPlayer) + "/" + std::to_string(room.maxPlayer);
    std::string statusText = room.playing ? "Playing" : "Waiting";

    const int noWidth = 6;
    const int codeWidth = 12;
    const int roomNameWidth = 22;
    const int playersWidth = 9;

    if (getDisplayWidth(roomName) > roomNameWidth - 1) {
        roomName = roomName.substr(0, roomNameWidth - 1);
    }

    return noText + makePadding(noText, noWidth) +
        codeText + makePadding(codeText, codeWidth) +
        roomName + makePadding(roomName, roomNameWidth) +
        playersText + makePadding(playersText, playersWidth) +
        statusText;
}

std::string MultiplayerClimbingGame::selectCard(const std::vector<std::string>& availableCards,
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
    bool& cancelled) const {
    cancelled = false;
    std::vector<std::string> cardNames;
    for (const std::string& card : availableCards) {
        cardNames.push_back(protocolCardToDisplayName(card));
    }

    int selected = 0;
    ClimbingGameView view;

    while (true) {
        view.drawGameScreen(
            cardNames,
            selected,
            p1RoundScore,
            p2RoundScore,
            roundNumber,
            maxRounds,
            setNumber,
            maxSets,
            p1SetWins,
            p2SetWins,
            "Online",
            localName,
            opponentName
        );

        int key = _getch();

        if (key == 224 || key == 0) {
            int arrow = _getch();
            if (arrow == 72) {
                selected = (selected - 1 + (int)cardNames.size()) % (int)cardNames.size();
            }
            else if (arrow == 80) {
                selected = (selected + 1) % (int)cardNames.size();
            }
        }
        else if (key == 13) {
            return availableCards[selected];
        }
        else if (key == 27) {
            PauseMenuView pauseMenu;
            State::PauseResult result = pauseMenu.runOnline();
            if (result == State::PauseResult::EXIT_TO_MENU) {
                cancelled = true;
                return "";
            }
        }
    }
}

void MultiplayerClimbingGame::showDisconnectedMessage(const std::string& playerName) const {
    system("cls");
    system("color 0E");

    std::cout << "==================== CONNECTION LOST ====================\n\n";

    if (playerName.empty()) {
        std::cout << "상대 플레이어와 연결이 끊어졌습니다.\n";
    }
    else {
        std::cout << playerName << " 플레이어와 연결이 끊어졌습니다.\n";
    }

    std::cout << "\n게임을 종료하고 온라인 메뉴로 돌아갑니다.\n";
    std::cout << "\nPress any key to return...";
    _getch();
}

std::string MultiplayerClimbingGame::protocolCardToDisplayName(const std::string& card) const {
    if (card == "KING") {
        return "King";
    }
    if (card == "QUEEN") {
        return "Queen";
    }
    if (card == "SERVANT") {
        return "Servant";
    }
    return "Joker";
}

int MultiplayerClimbingGame::getDisplayWidth(const std::string& text) const {
    int width = 0;

    for (size_t i = 0; i < text.size(); i++) {
        unsigned char ch = static_cast<unsigned char>(text[i]);

        if (ch < 0x80) {
            width++;
        }
        else if ((ch & 0xC0) == 0x80) {
            continue;
        }
        else {
            width += 2;
        }
    }

    return width;
}

std::string MultiplayerClimbingGame::makePadding(const std::string& text, int targetWidth) const {
    int padding = targetWidth - getDisplayWidth(text);

    if (padding < 0) {
        padding = 0;
    }

    return std::string(padding, ' ');
}
