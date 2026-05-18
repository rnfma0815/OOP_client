#include "MultiplayerBlackjackGame.h"
#include "NetworkClient.h"
#include "BlackjackView.h"
#include "PauseMenuView.h"
#include "../common/ProtocolUtil.h"

#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <sstream>

MultiplayerBlackjackGame::MultiplayerBlackjackGame()
    : serverHost("127.0.0.1"), serverPort(9000) {
    menuItems.push_back("Create Room");
    menuItems.push_back("Room List");
    menuItems.push_back("Description");
    menuItems.push_back("Back");
}

void MultiplayerBlackjackGame::run() {
    int selected = 0;

    while (true) {
        drawMenu(selected);
        int key = _getch();

        if (key == 224 || key == 0) {
            int arrow = _getch();
            if (arrow == 72) selected = (selected - 1 + (int)menuItems.size()) % (int)menuItems.size();
            else if (arrow == 80) selected = (selected + 1) % (int)menuItems.size();
        }
        else if (key == 13) {
            if (selected == 0) createRoom();
            else if (selected == 1) showRoomList();
            else if (selected == 2) showDescription();
            else break;
        }
        else if (key == 27) {
            break;
        }
    }
}

void MultiplayerBlackjackGame::drawMenu(int selected) const {
    system("cls");
    system("color 0E");

    std::cout << "========================================================\n";
    std::cout << "                <  Online Blackjack  >\n";
    std::cout << "========================================================\n\n";

    std::cout << "         ┌────────┐  ┌────────┐  ┌────────┐\n";
    std::cout << "         │A       │  │K       │  │Q       │\n";
    std::cout << "         │        │  │        │  │        │\n";
    std::cout << "         │   ♠    │  │   ♥    │  │   ♦    │\n";
    std::cout << "         │        │  │        │  │        │\n";
    std::cout << "         │       A│  │       K│  │       Q│\n";
    std::cout << "         └────────┘  └────────┘  └────────┘\n\n";

    for (int i = 0; i < (int)menuItems.size(); i++) {
        if (i == selected) std::cout << "                     ▶ " << menuItems[i] << " ◀\n";
        else std::cout << "                       " << menuItems[i] << "\n";
    }

    std::cout << "\n              Use UP / DOWN and press ENTER\n";
}

void MultiplayerBlackjackGame::drawRoomList(const std::vector<RoomInfo>& rooms, int selected, const std::string& status) const {
    system("cls");
    system("color 0E");

    const int boxWidth = 58;
    const int listAreaHeight = 10;
    std::string horizontalLine;
    for (int i = 0; i < boxWidth; i++) horizontalLine += "─";

    std::cout << "====================== BLACKJACK ROOM LIST ======================\n\n";
    std::cout << " ┌" << horizontalLine << "┐\n";

    int printedRows = 0;
    if (rooms.empty()) {
        std::string text = "방이 비어있습니다.";
        std::cout << " │ " << text << makePadding(text, boxWidth - 1) << "│\n";
        printedRows++;
    }
    else {
        std::string header = "No.  Code        Room Name          Players  Status";
        std::cout << " │ " << header << makePadding(header, boxWidth - 1) << "│\n";
        std::cout << " ├" << horizontalLine << "┤\n";
        printedRows += 2;

        int maxVisibleRooms = listAreaHeight - printedRows;
        int firstRoom = 0;
        if (selected >= maxVisibleRooms) firstRoom = selected - maxVisibleRooms + 1;
        if (firstRoom + maxVisibleRooms > (int)rooms.size()) firstRoom = (int)rooms.size() - maxVisibleRooms;
        if (firstRoom < 0) firstRoom = 0;

        int lastRoom = firstRoom + maxVisibleRooms;
        if (lastRoom > (int)rooms.size()) lastRoom = (int)rooms.size();

        for (int i = firstRoom; i < lastRoom; i++) {
            const RoomInfo& room = rooms[i];
            std::string marker = selected == i ? "> " : "  ";
            std::string noText = marker + std::to_string(i + 1);
            std::string players = std::to_string(room.currentPlayer) + "/" + std::to_string(room.maxPlayer);
            std::string statusText = room.playing ? "Playing" : "Waiting";
            std::string roomName = room.name;
            if (getDisplayWidth(roomName) > 21) roomName = roomName.substr(0, 21);

            std::string row =
                noText + makePadding(noText, 6) +
                room.code + makePadding(room.code, 12) +
                roomName + makePadding(roomName, 22) +
                players + makePadding(players, 9) +
                statusText;

            std::cout << " │ " << row << makePadding(row, boxWidth - 1) << "│\n";
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

    std::cout << (selected == joinCodeIndex ? " ▶ Join by Code ◀\n" : "   Join by Code\n");
    std::cout << (selected == refreshIndex ? " ▶ Refresh ◀\n" : "   Refresh\n");
    std::cout << (selected == backIndex ? " ▶ Back ◀\n" : "   Back\n");

    std::cout << "\n";
    if (!status.empty()) std::cout << " " << status << "\n";
    std::cout << " Use UP / DOWN to move menu\n";
    std::cout << " Press ENTER to select, R to refresh\n";
}

void MultiplayerBlackjackGame::drawWaitingRoom(const std::string& roomCode, const std::string& roomName, const std::string& nickname, const std::string& status) const {
    system("cls");
    system("color 0E");

    const int boxWidth = 58;
    std::string horizontalLine;
    for (int i = 0; i < boxWidth; i++) horizontalLine += "─";

    std::cout << "==================== BLACKJACK WAITING ROOM ====================\n\n";
    std::cout << " ┌" << horizontalLine << "┐\n";
    std::string header = "No.  Code        Room Name          Players  Status";
    std::cout << " │ " << header << makePadding(header, boxWidth - 1) << "│\n";
    std::cout << " │" << std::string(boxWidth, ' ') << "│\n";

    std::string players = "1/2";
    std::string statusText = "Waiting";
    std::string row =
        std::string("1") + makePadding("1", 6) +
        roomCode + makePadding(roomCode, 12) +
        roomName + makePadding(roomName, 22) +
        players + makePadding(players, 9) +
        statusText;
    std::cout << " │ " << row << makePadding(row, boxWidth - 1) << "│\n";
    std::cout << " ├" << horizontalLine << "┤\n";
    std::cout << " │ " << nickname << makePadding(nickname, boxWidth - 1) << "│\n";
    std::string waiting = "waiting...";
    std::cout << " │ " << waiting << makePadding(waiting, boxWidth - 1) << "│\n";
    for (int i = 0; i < 4; i++) std::cout << " │" << std::string(boxWidth, ' ') << "│\n";
    std::cout << " └" << horizontalLine << "┘\n\n";
    std::cout << " ▶ Back ◀\n\n";
    if (!status.empty()) std::cout << " " << status << "\n";
    std::cout << " Press ENTER to leave room\n";
}

void MultiplayerBlackjackGame::createRoom() {
    system("cls");
    system("color 0E");

    std::cout << "===================== CREATE BLACKJACK ROOM =====================\n\n";
    std::string nickname = readText("Nickname", "Player1");
    if (nickname == "__CANCEL__") return;
    std::string roomName = readText("Room name", "Blackjack Room");
    if (roomName == "__CANCEL__") return;

    NetworkClient client;
    if (!client.connectToServer(serverHost, serverPort)) {
        std::cout << "\nFailed to connect to server. Run Server.exe first.\n";
        std::cout << "\nPress any key to return...";
        _getch();
        return;
    }

    client.sendMessage("CREATE_ROOM|" + ProtocolUtil::encode(roomName) + "|" + ProtocolUtil::encode(nickname) + "|BLACKJACK");

    std::string response;
    if (!client.receiveLine(response) || response.rfind("ROOM_CREATED|", 0) != 0) {
        std::cout << "\nServer: " << response << "\n";
        std::cout << "\nPress any key to return...";
        _getch();
        return;
    }

    std::vector<std::string> fields = split(response, '|');
    std::string roomCode = fields.size() > 1 ? fields[1] : "";

    drawWaitingRoom(roomCode, roomName, nickname, "");

    while (client.isConnected()) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 13 || key == 27) {
                client.sendMessage("LEAVE_ROOM");
                client.disconnect();
                return;
            }
        }

        if (client.receiveLineWithTimeout(response, 100)) {
            if (response.rfind("GAME_START|", 0) == 0) {
                waitForGameStart(client, nickname);
                return;
            }
            if (response.rfind("PLAYER_JOINED|", 0) == 0) {
                std::vector<std::string> joinFields = split(response, '|');
                std::string joinedName = joinFields.size() > 1 ? ProtocolUtil::decode(joinFields[1]) : "Player2";
                drawWaitingRoom(roomCode, roomName, nickname, joinedName + " joined. Starting game...");
            }
        }
    }

    showDisconnectedMessage("");
}

void MultiplayerBlackjackGame::showRoomList() {
    std::vector<RoomInfo> rooms;
    std::string status;
    int selected = 0;

    fetchRoomList(rooms, status);

    while (true) {
        int itemCount = (int)rooms.size() + 3;
        if (selected >= itemCount) selected = itemCount - 1;

        drawRoomList(rooms, selected, status);
        int key = _getch();

        if (key == 224 || key == 0) {
            int arrow = _getch();
            if (arrow == 72) selected = (selected - 1 + itemCount) % itemCount;
            else if (arrow == 80) selected = (selected + 1) % itemCount;
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
                system("cls");
                system("color 0E");
                std::cout << "==================== JOIN BY CODE ====================\n\n";
                std::string roomCode = readText("Room code", "");
                if (roomCode != "__CANCEL__" && !roomCode.empty()) joinRoom(roomCode, "Blackjack Room");
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
                if (rooms[selected].playing || rooms[selected].currentPlayer >= rooms[selected].maxPlayer) {
                    status = "This room is not available.";
                    continue;
                }
                joinRoom(rooms[selected].code, rooms[selected].name);
                fetchRoomList(rooms, status);
                selected = 0;
            }
        }
        else if (key == 27) {
            break;
        }
    }
}

void MultiplayerBlackjackGame::joinRoom(const std::string& roomCode, const std::string& roomName) {
    system("cls");
    system("color 0E");

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
    if (!client.receiveLine(response) || response.rfind("ROOM_JOINED|", 0) != 0) {
        std::cout << "\nServer: " << response << "\n";
        std::cout << "\nPress any key to return...";
        _getch();
        return;
    }

    while (client.receiveLine(response)) {
        if (response.rfind("GAME_START|", 0) == 0) {
            waitForGameStart(client, nickname);
            return;
        }

        if (response.rfind("PLAYER_LEFT|", 0) == 0 || response.rfind("GAME_ABORTED|", 0) == 0) {
            std::vector<std::string> fields = split(response, '|');
            showDisconnectedMessage(fields.size() > 1 ? ProtocolUtil::decode(fields[1]) : "");
            return;
        }
    }

    showDisconnectedMessage("");
}

void MultiplayerBlackjackGame::waitForGameStart(NetworkClient& client, const std::string& nickname) const {
    std::string firstState;
    while (client.receiveLine(firstState)) {
        if (firstState.rfind("BLACKJACK_STATE|", 0) == 0) {
            std::vector<std::string> fields = split(firstState, '|');
            playMatch(client, nickname, firstState);
            return;
        }

        if (firstState.rfind("PLAYER_LEFT|", 0) == 0 || firstState.rfind("GAME_ABORTED|", 0) == 0) {
            std::vector<std::string> fields = split(firstState, '|');
            showDisconnectedMessage(fields.size() > 1 ? ProtocolUtil::decode(fields[1]) : "");
            return;
        }
    }

    showDisconnectedMessage("");
}

void MultiplayerBlackjackGame::playMatch(NetworkClient& client, const std::string& nickname, const std::string& firstState) const {
    BlackjackView view;
    int selectedMenu = 0;
    std::string pendingMessage = firstState;
    std::vector<std::string> actionLogs;
    std::string lastActionKey;
    int logRoundNumber = 0;

    auto addLog = [&actionLogs](const std::string& text) {
        if (text.empty()) return;
        if (!actionLogs.empty() && actionLogs.back() == text) return;
        actionLogs.push_back(text);
        if (actionLogs.size() > 3) {
            actionLogs.erase(actionLogs.begin());
        }
    };

    auto removeWaitingLog = [&actionLogs]() {
        if (!actionLogs.empty() && actionLogs.back() == "Waiting...") {
            actionLogs.pop_back();
        }
    };

    auto addWaitingLog = [&actionLogs, &removeWaitingLog]() {
        removeWaitingLog();
        actionLogs.push_back("Waiting...");
        if (actionLogs.size() > 3) {
            actionLogs.erase(actionLogs.begin());
        }
    };

    auto actionName = [](const std::string& action) {
        if (action == "HIT") return std::string("Hit");
        if (action == "STAND") return std::string("Stand");
        if (action == "HIDDEN") return std::string("Hidden Card");
        return std::string("");
    };

    while (client.isConnected()) {
        std::string message;
        if (!pendingMessage.empty()) {
            message = pendingMessage;
            pendingMessage.clear();
        }
        else {
            if (!client.receiveLine(message)) {
                showDisconnectedMessage("");
                break;
            }
        }

        if (message.rfind("BLACKJACK_STATE|", 0) == 0) {
            std::vector<std::string> fields = split(message, '|');
            if (fields.size() < 11) {
                continue;
            }

            std::string serverP1Name = ProtocolUtil::decode(fields[1]);
            std::string serverP2Name = ProtocolUtil::decode(fields[2]);
            bool amP1 = nickname == serverP1Name;
            std::string localName = amP1 ? serverP1Name : serverP2Name;
            std::string opponentName = amP1 ? serverP2Name : serverP1Name;
            int roundNumber = std::atoi(fields[3].c_str());
            int maxRounds = std::atoi(fields[4].c_str());
            int p1Point = std::atoi(fields[5].c_str());
            int p2Point = std::atoi(fields[6].c_str());
            int localPoint = amP1 ? p1Point : p2Point;
            int opponentPoint = amP1 ? p2Point : p1Point;
            std::vector<Card> localCards = parseCards(fields[7]);
            std::vector<Card> hiddenCards = parseCards(fields[8]);
            std::vector<Card> dealerCards = parseCards(fields[9]);
            std::string opponentFirst = fields[10] == "-" ? "(Hidden)" : parseCard(fields[10]).toString();
            std::string playerStatus = fields.size() > 11 ? fields[11] : "ACT";
            std::string opponentStatus = fields.size() > 12 ? fields[12] : "ACT";
            std::string playerActionLog = fields.size() > 13 ? fields[13] : "";
            std::string opponentActionLog = fields.size() > 14 ? fields[14] : "";

            if (roundNumber != logRoundNumber) {
                actionLogs.clear();
                lastActionKey.clear();
                logRoundNumber = roundNumber;
            }

            std::string actionKey =
                std::to_string(roundNumber) + "|" +
                playerStatus + "|" +
                opponentStatus + "|" +
                playerActionLog + "|" +
                opponentActionLog;

            if (actionKey != lastActionKey) {
                std::string opponentActionName = actionName(opponentActionLog);
                bool waitingForOpponent =
                    playerStatus != "ACT" &&
                    opponentStatus == "ACT";

                if (!opponentActionName.empty() && !waitingForOpponent) {
                    removeWaitingLog();
                    addLog("상대방이 " + opponentActionName + "를 선택하였습니다.");
                }
                if (waitingForOpponent) {
                    addWaitingLog();
                }
                lastActionKey = actionKey;
            }

            if (playerStatus == "ACT") {
                removeWaitingLog();
            }

            if (playerStatus != "ACT") {
                view.drawMultiplayerGameScreen(
                    localName,
                    opponentName,
                    localCards,
                    hiddenCards,
                    dealerCards,
                    opponentFirst,
                    roundNumber,
                    maxRounds,
                    localPoint,
                    opponentPoint,
                    selectedMenu,
                    actionLogs
                );

                if (_kbhit()) {
                    int key = _getch();
                    if (key == 27) {
                        PauseMenuView pauseMenu;
                        State::PauseResult result = pauseMenu.runOnline();
                        if (result == State::PauseResult::EXIT_TO_MENU) {
                            client.sendMessage("LEAVE_ROOM");
                            client.disconnect();
                            return;
                        }
                    }
                }
                continue;
            }

            bool acted = false;
            while (!acted) {
                view.drawMultiplayerGameScreen(
                    localName,
                    opponentName,
                    localCards,
                    hiddenCards,
                    dealerCards,
                    opponentFirst,
                    roundNumber,
                    maxRounds,
                    localPoint,
                    opponentPoint,
                    selectedMenu,
                    actionLogs
                );

                bool redraw = false;
                while (!acted && !redraw) {
                    std::string liveMessage;
                    if (client.receiveLineWithTimeout(liveMessage, 50)) {
                        pendingMessage = liveMessage;
                        acted = true;
                        break;
                    }

                    if (!_kbhit()) {
                        continue;
                    }

                    int key = _getch();
                    if (key == 224 || key == 0) {
                        int arrow = _getch();
                        if (arrow == 75) selectedMenu = (selectedMenu - 1 + 3) % 3;
                        else if (arrow == 77) selectedMenu = (selectedMenu + 1) % 3;
                        redraw = true;
                    }
                    else if (key == 13) {
                        if (selectedMenu == 0) {
                            client.sendMessage("BLACKJACK_ACTION|OPEN");
                            addWaitingLog();
                            view.drawMultiplayerGameScreen(
                                localName,
                                opponentName,
                                localCards,
                                hiddenCards,
                                dealerCards,
                                opponentFirst,
                                roundNumber,
                                maxRounds,
                                localPoint,
                                opponentPoint,
                                selectedMenu,
                                actionLogs
                            );
                            acted = true;
                        }
                        else if (selectedMenu == 1) {
                            client.sendMessage("BLACKJACK_ACTION|STOP");
                            addWaitingLog();
                            view.drawMultiplayerGameScreen(
                                localName,
                                opponentName,
                                localCards,
                                hiddenCards,
                                dealerCards,
                                opponentFirst,
                                roundNumber,
                                maxRounds,
                                localPoint,
                                opponentPoint,
                                selectedMenu,
                                actionLogs
                            );
                            acted = true;
                        }
                        else {
                            if (hiddenCards.empty()) {
                                redraw = true;
                                continue;
                            }
                            client.sendMessage("BLACKJACK_ACTION|USE_HIDDEN|0");
                            addWaitingLog();
                            view.drawMultiplayerGameScreen(
                                localName,
                                opponentName,
                                localCards,
                                hiddenCards,
                                dealerCards,
                                opponentFirst,
                                roundNumber,
                                maxRounds,
                                localPoint,
                                opponentPoint,
                                selectedMenu,
                                actionLogs
                            );
                            acted = true;
                        }
                    }
                    else if (key == 27) {
                        PauseMenuView pauseMenu;
                        State::PauseResult result = pauseMenu.runOnline();
                        if (result == State::PauseResult::EXIT_TO_MENU) {
                            client.sendMessage("LEAVE_ROOM");
                            client.disconnect();
                            return;
                        }
                        redraw = true;
                    }
                }
            }
        }
        else if (message.rfind("BLACKJACK_ROUND_RESULT|", 0) == 0) {
            std::vector<std::string> fields = split(message, '|');
            if (fields.size() < 13) {
                continue;
            }

            std::string serverP1Name = ProtocolUtil::decode(fields[1]);
            std::string serverP2Name = ProtocolUtil::decode(fields[4]);
            bool amP1 = nickname == serverP1Name;
            std::string localName = amP1 ? serverP1Name : serverP2Name;
            std::string opponentName = amP1 ? serverP2Name : serverP1Name;
            std::vector<Card> p1Cards = parseCards(fields[2]);
            std::vector<Card> p2Cards = parseCards(fields[5]);
            std::string p1Result = fields[3];
            std::string p2Result = fields[6];
            std::vector<Card> dealerCards = parseCards(fields[7]);
            int roundNumber = std::atoi(fields[9].c_str());
            int maxRounds = std::atoi(fields[10].c_str());
            int p1Point = std::atoi(fields[11].c_str());
            int p2Point = std::atoi(fields[12].c_str());

            view.showMultiplayerRoundResult(
                localName,
                opponentName,
                amP1 ? p1Cards : p2Cards,
                amP1 ? p2Cards : p1Cards,
                dealerCards,
                amP1 ? p1Result : p2Result,
                amP1 ? p2Result : p1Result,
                roundNumber,
                maxRounds,
                amP1 ? p1Point : p2Point,
                amP1 ? p2Point : p1Point
            );
        }
        else if (message.rfind("BLACKJACK_GAME_END|", 0) == 0) {
            std::vector<std::string> fields = split(message, '|');
            std::string winner = fields.size() > 1 ? ProtocolUtil::decode(fields[1]) : "DRAW";
            system("cls");
            system("color 0E");
            std::cout << "====================== BLACKJACK GAME END ======================\n\n";
            if (winner == "DRAW") std::cout << "Final result is draw.\n";
            else std::cout << winner << " wins the match!\n";
            std::cout << "\nPress any key to return...";
            _getch();
            return;
        }
        else if (message.rfind("PLAYER_LEFT|", 0) == 0 || message.rfind("GAME_ABORTED|", 0) == 0) {
            std::vector<std::string> fields = split(message, '|');
            showDisconnectedMessage(fields.size() > 1 ? ProtocolUtil::decode(fields[1]) : "");
            return;
        }
    }
}

void MultiplayerBlackjackGame::showDisconnectedMessage(const std::string& playerName) const {
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

void MultiplayerBlackjackGame::showDescription() const {
    system("cls");
    system("color 0E");

    std::cout << "================ ONLINE BLACKJACK DESCRIPTION ================\n\n";
    std::cout << "- 2 players play directly against each other without a dealer.\n";
    std::cout << "- Hit / Stand choices are collected, then resolved together.\n";
    std::cout << "- Bust is judged only after both players stand; double bust has no winner.\n";

    std::cout << "\n\nPress any key to return...";
    _getch();
}

bool MultiplayerBlackjackGame::fetchRoomList(std::vector<RoomInfo>& rooms, std::string& status) const {
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

std::vector<MultiplayerBlackjackGame::RoomInfo> MultiplayerBlackjackGame::parseRoomList(const std::string& message) const {
    std::vector<RoomInfo> rooms;
    const std::string prefix = "ROOM_LIST|";
    if (message.rfind(prefix, 0) != 0) return rooms;

    std::string payload = message.substr(prefix.size());
    if (payload.empty()) return rooms;

    std::vector<std::string> roomItems = split(payload, ';');
    for (const std::string& item : roomItems) {
        std::vector<std::string> fields = split(item, ',');
        if (fields.size() < 5) continue;

        RoomInfo room;
        room.code = fields[0];
        room.name = ProtocolUtil::decode(fields[1]);
        room.currentPlayer = std::atoi(fields[2].c_str());
        room.maxPlayer = std::atoi(fields[3].c_str());
        room.playing = fields[4] == "true";
        room.playerNames = fields.size() >= 6 ? ProtocolUtil::decode(fields[5]) : "";
        room.gameType = fields.size() >= 7 ? fields[6] : "CLIMBING";

        if (room.gameType == "BLACKJACK") {
            rooms.push_back(room);
        }
    }

    return rooms;
}

std::vector<Card> MultiplayerBlackjackGame::parseCards(const std::string& text) const {
    std::vector<Card> cards;
    if (text.empty() || text == "-") {
        return cards;
    }

    std::vector<std::string> items = split(text, ',');
    for (const std::string& item : items) {
        if (!item.empty()) {
            cards.push_back(parseCard(item));
        }
    }

    return cards;
}

Card MultiplayerBlackjackGame::parseCard(const std::string& text) const {
    if (text.size() < 2) {
        return Card();
    }

    std::string rank = text.substr(0, text.size() - 1);
    char suitChar = text.back();
    std::string suit = "Spade";

    if (suitChar == 'D') suit = "Diamond";
    else if (suitChar == 'H') suit = "Heart";
    else if (suitChar == 'C') suit = "Clover";

    return Card(rank, suit);
}

std::vector<std::string> MultiplayerBlackjackGame::split(const std::string& text, char delimiter) const {
    return ProtocolUtil::split(text, delimiter);
}

std::string MultiplayerBlackjackGame::readText(const std::string& label, const std::string& defaultValue) const {
    std::string value;

    if (defaultValue.empty()) std::cout << label << ": ";
    else std::cout << label << " [" << defaultValue << "]: ";

    while (true) {
        int key = _getch();
        if (key == 27) return "__CANCEL__";
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

    if (value.empty()) return defaultValue;
    return value;
}

int MultiplayerBlackjackGame::getDisplayWidth(const std::string& text) const {
    int width = 0;
    for (size_t i = 0; i < text.size(); i++) {
        unsigned char ch = static_cast<unsigned char>(text[i]);
        if (ch < 0x80) width++;
        else if ((ch & 0xC0) == 0x80) continue;
        else width += 2;
    }
    return width;
}

std::string MultiplayerBlackjackGame::makePadding(const std::string& text, int targetWidth) const {
    int padding = targetWidth - getDisplayWidth(text);
    if (padding < 0) padding = 0;
    return std::string(padding, ' ');
}
