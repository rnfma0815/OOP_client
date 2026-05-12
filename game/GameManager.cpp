#include "GameManager.h"
#include "Blackjack.h"
#include "MemoryGame.h"
#include "ClimbingGame.h"
#include "MultiplayerClimbingGame.h"
#include "MultiplayerBlackjackGame.h"

#include <iostream>
#include <conio.h>
#include <windows.h>

GameManager::GameManager() {
    games.push_back(new Blackjack());
    games.push_back(new MemoryGame());
    games.push_back(new ClimbingGame());

    lobbyMenu.push_back("Start Game");
    lobbyMenu.push_back("Options");
    lobbyMenu.push_back("Exit");

    playModeMenu.push_back("Online Mode");
    playModeMenu.push_back("Offline Mode");
    playModeMenu.push_back("Back");

    onlineGameMenu.push_back("Create Room");
    onlineGameMenu.push_back("Room List");
    onlineGameMenu.push_back("Description");
    onlineGameMenu.push_back("Back");

    gameMenu.push_back("Start");
    gameMenu.push_back("Difficulty");
    gameMenu.push_back("Description");
    gameMenu.push_back("Back");
}

GameManager::~GameManager() {
    for (Game* game : games) {
        delete game;
    }
}

void GameManager::drawLobbyMenu(int selected) {
    system("cls");
    system("color 0E");

    std::cout << R"(

       ┌────────┐  ┌────────┐  ┌────────┐  ┌────────┐
       │A       │  │A       │  │A       │  │A       │
       │        │  │        │  │        │  │        │
       │   ♠    │  │   ♥    │  │   ♦    │  │   ♣    │
       │        │  │        │  │        │  │        │
       │       A│  │       A│  │       A│  │       A│
       └────────┘  └────────┘  └────────┘  └────────┘

========================================================
                  G A M E   P A C K
========================================================

)";

    for (int i = 0; i < (int)lobbyMenu.size(); i++) {
        if (i == selected) {
            std::cout << "                     ▶ " << lobbyMenu[i] << " ◀\n";
        }
        else {
            std::cout << "                       " << lobbyMenu[i] << "\n";
        }
    }

    std::cout << "\n              Use UP / DOWN and press ENTER\n";
}

int GameManager::selectLobbyMenu() {
    int selected = 0;

    while (true) {
        drawLobbyMenu(selected);

        int key = _getch();

        if (key == 224 || key == 0) {
            int arrow = _getch();

            if (arrow == 72) {
                selected = (selected - 1 + (int)lobbyMenu.size()) % (int)lobbyMenu.size();
            }
            else if (arrow == 80) {
                selected = (selected + 1) % (int)lobbyMenu.size();
            }
        }
        else if (key == 13) {
            return selected;
        }
    }
}

void GameManager::drawPlayModeMenu(int selected) {
    system("cls");
    system("color 0E");

    std::cout << R"(

========================================================
                    S E L E C T   M O D E
========================================================

)";

    for (int i = 0; i < (int)playModeMenu.size(); i++) {
        if (i == selected) {
            std::cout << "                     ▶ " << playModeMenu[i] << " ◀\n";
        }
        else {
            std::cout << "                       " << playModeMenu[i] << "\n";
        }
    }

    std::cout << "\n              Use UP / DOWN and press ENTER\n";
}

void GameManager::runPlayModeMenu() {
    int selected = 0;

    while (true) {
        drawPlayModeMenu(selected);

        int key = _getch();

        if (key == 224 || key == 0) {
            int arrow = _getch();

            if (arrow == 72) {
                selected = (selected - 1 + (int)playModeMenu.size()) % (int)playModeMenu.size();
            }
            else if (arrow == 80) {
                selected = (selected + 1) % (int)playModeMenu.size();
            }
        }
        else if (key == 13) {
            if (selected == 0) {
                showOnlineMode();
            }
            else if (selected == 1) {
                runGameSelectMenu();
            }
            else if (selected == 2) {
                break;
            }
        }
        else if (key == 27) {
            break;
        }
    }
}

void GameManager::drawGameSelectScreen(int selectedGame, int selectedMenu) {
    system("cls");
    system("color 0E");

    std::string title;
    std::vector<std::string> art;

    if (selectedGame == 0) {
        title = "Blackjack";
        art = {
            "         ┌────────┐  ┌────────┐  ┌────────┐",
            "         │6       │  │7       │  │8       │",
            "         │        │  │        │  │        │",
            "         │   ♠    │  │   ♥    │  │   ♦    │",
            "         │        │  │        │  │        │",
            "         │       6│  │       7│  │       8│",
            "         └────────┘  └────────┘  └────────┘",
            "",
            "                   6 + 7 + 8 = 21"
        };
    }
    else if (selectedGame == 1) {
        title = "Memory Game";
        art = {
            "                ┌─┐   ┌─┐   ┌─┐   ┌─┐",
            "                │?│   │?│   │?│   │?│",
            "                └─┘   └─┘   └─┘   └─┘",
            "",
            "                ┌─┐   ┌─┐   ┌─┐   ┌─┐",
            "                │?│   │?│   │?│   │?│",
            "                └─┘   └─┘   └─┘   └─┘",
            "",
            ""
        };
    }
    else {
        title = "Climbing Game";
        art = {
            "       ┌────────┐  ┌────────┐  ┌────────┐  ┌────────┐",
            "       │K       │  │Q       │  │J       │  │JOKER   │",
            "       │        │  │        │  │        │  │        │",
            "       │   ♠    │  │   ♥    │  │   ♦    │  │   ★    │",
            "       │        │  │        │  │        │  │        │",
            "       │       K│  │       Q│  │       J│  │   JOKER│",
            "       └────────┘  └────────┘  └────────┘  └────────┘",
            "",
            ""
        };
    }

    std::cout << "========================================================\n";
    std::cout << "                  ◀  " << title << "  ▶\n";
    std::cout << "========================================================\n\n";

    for (const std::string& line : art) {
        std::cout << line << "\n";
    }

    std::cout << "\n";

    for (int i = 0; i < (int)gameMenu.size(); i++) {
        std::string menuText;

        if (gameMenu[i] == "Difficulty") {
            menuText = "Difficulty : " + getDifficultyString();
        }
        else {
            menuText = gameMenu[i];
        }

        if (i == selectedMenu) {
            std::cout << "                     ▶ " << menuText << " ◀\n";
        }
        else {
            std::cout << "                       " << menuText << "\n";
        }
    }

    std::cout << "\n";
    std::cout << "        Use LEFT / RIGHT to change game\n";
    std::cout << "        Use UP / DOWN to move menu\n";
    std::cout << "        Press ENTER to select\n";
}

std::string GameManager::getDifficultyString() {
    switch (difficulty) {
    case State::Difficulty::EASY:   return "Easy";
    case State::Difficulty::NORMAL: return "Normal";
    case State::Difficulty::HARD:   return "Hard";
    default: return "Normal";
    }
}

void GameManager::changeDifficulty(int direction) {
    int d = static_cast<int>(difficulty);
    d = (d + direction + 3) % 3;
    difficulty = static_cast<State::Difficulty>(d);
}

void GameManager::showDescription(int gameIndex) {
    system("cls");
    system("color 0E");

    if (gameIndex == 0) {
        std::cout << "================ BLACKJACK DESCRIPTION ================\n\n";
        std::cout << "- BLACKJACK 설명.\n";
    }
    else if (gameIndex == 1) {
        std::cout << "================ MEMORY GAME DESCRIPTION ================\n\n";
        std::cout << "- MEMORY GAME 설명.\n";
    }
    else if (gameIndex == 2) {
        std::cout << "=============== CLIMBING GAME DESCRIPTION ===============\n\n";
        std::cout << "- CLIMBING GAME 설명.\n";
    }

    std::cout << "\n\nPress any key to return...";
    _getch();
}

void GameManager::showOption() {
    system("cls");
    system("color 0E");

    std::cout << "===================== OPTIONS =====================\n\n";
    std::cout << "Options screen is not implemented yet.\n";
    std::cout << "You can add settings here later.\n";

    std::cout << "\n\nPress any key to return...";
    _getch();
}

void GameManager::showOnlineMode() {
    runOnlineGameMenu();
}

void GameManager::drawOnlineGameSelectScreen(int selectedGame, int selectedMenu) {
    system("cls");
    system("color 0E");

    std::string title;
    std::vector<std::string> art;

    if (selectedGame == 0) {
        title = "Blackjack";
        art = {
            "         ┌────────┐  ┌────────┐  ┌────────┐",
            "         │A       │  │K       │  │Q       │",
            "         │        │  │        │  │        │",
            "         │   ♠    │  │   ♥    │  │   ♦    │",
            "         │        │  │        │  │        │",
            "         │       A│  │       K│  │       Q│",
            "         └────────┘  └────────┘  └────────┘",
            "",
            ""
        };
    }
    else {
        title = "Climbing Game";
        art = {
            "       ┌────────┐  ┌────────┐  ┌────────┐  ┌────────┐",
            "       │K       │  │Q       │  │J       │  │JOKER   │",
            "       │        │  │        │  │        │  │        │",
            "       │   ♠    │  │   ♥    │  │   ♦    │  │   ★    │",
            "       │        │  │        │  │        │  │        │",
            "       │       K│  │       Q│  │       J│  │   JOKER│",
            "       └────────┘  └────────┘  └────────┘  └────────┘",
            "",
            ""
        };
    }

    std::cout << "========================================================\n";
    std::cout << "              ONLINE  ◀  " << title << "  ▶\n";
    std::cout << "========================================================\n\n";

    for (const std::string& line : art) {
        std::cout << line << "\n";
    }

    std::cout << "\n";

    for (int i = 0; i < (int)onlineGameMenu.size(); i++) {
        if (i == selectedMenu) {
            std::cout << "                     ▶ " << onlineGameMenu[i] << " ◀\n";
        }
        else {
            std::cout << "                       " << onlineGameMenu[i] << "\n";
        }
    }
    std::cout << "\n";
    std::cout << "        Use LEFT / RIGHT to change game\n";
    std::cout << "        Use UP / DOWN to move menu\n";
    std::cout << "        Press ENTER to select\n";
}

void GameManager::runOnlineGameMenu() {
    int selectedGame = 0;
    int selectedMenu = 0;
    const int onlineGameCount = 2;

    while (true) {
        drawOnlineGameSelectScreen(selectedGame, selectedMenu);

        int key = _getch();

        if (key == 224 || key == 0) {
            int arrow = _getch();
            if (arrow == 75) {
                selectedGame = (selectedGame - 1 + onlineGameCount) % onlineGameCount;
            }
            else if (arrow == 77) {
                selectedGame = (selectedGame + 1) % onlineGameCount;
            }
            else if (arrow == 72) {
                selectedMenu = (selectedMenu - 1 + (int)onlineGameMenu.size()) % (int)onlineGameMenu.size();
            }
            else if (arrow == 80) {
                selectedMenu = (selectedMenu + 1) % (int)onlineGameMenu.size();
            }
        }
        else if (key == 13) {
            if (selectedMenu == 0) {
                if (selectedGame == 0) {
                    MultiplayerBlackjackGame onlineBlackjackGame;
                    onlineBlackjackGame.createRoom();
                }
                else {
                    MultiplayerClimbingGame onlineClimbingGame;
                    onlineClimbingGame.createRoom();
                }
            }
            else if (selectedMenu == 1) {
                if (selectedGame == 0) {
                    MultiplayerBlackjackGame onlineBlackjackGame;
                    onlineBlackjackGame.showRoomList();
                }
                else {
                    MultiplayerClimbingGame onlineClimbingGame;
                    onlineClimbingGame.showRoomList();
                }
            }
            else if (selectedMenu == 2) {
                if (selectedGame == 0) {
                    MultiplayerBlackjackGame onlineBlackjackGame;
                    onlineBlackjackGame.showDescription();
                }
                else {
                    MultiplayerClimbingGame onlineClimbingGame;
                    onlineClimbingGame.showDescription();
                }
            }
            else {
                break;
            }
        }
        else if (key == 27) {
            break;
        }
    }
}

void GameManager::runGameSelectMenu() {
    int selectedGame = 0;
    int selectedMenu = 0;

    while (true) {
        drawGameSelectScreen(selectedGame, selectedMenu);

        int key = _getch();

        if (key == 224 || key == 0) {
            int arrow = _getch();

            if (arrow == 75) { // 왼쪽
                if (selectedMenu == 1) {
                    changeDifficulty(-1);
                }
                else {
                    selectedGame = (selectedGame - 1 + (int)games.size()) % (int)games.size();
                }
            }
            else if (arrow == 77) { // 오른쪽
                if (selectedMenu == 1) {
                    changeDifficulty(1);
                }
                else {
                    selectedGame = (selectedGame + 1) % (int)games.size();
                }
            }
            else if (arrow == 72) { // 위
                selectedMenu = (selectedMenu - 1 + (int)gameMenu.size()) % (int)gameMenu.size();
            }
            else if (arrow == 80) { // 아래
                selectedMenu = (selectedMenu + 1) % (int)gameMenu.size();
            }
        }
        else if (key == 13) { // Enter
            if (selectedMenu == 0) { // Start
                system("cls");
                games[selectedGame]->setDifficulty(difficulty);
                games[selectedGame]->play();
            }
            else if (selectedMenu == 2) { // Description
                showDescription(selectedGame);
            }
            else if (selectedMenu == 3) { // Back
                break;
            }
        }
        else if (key == 27) {
            break;
        }
    }
}

void GameManager::run() {
    while (true) {
        int choice = selectLobbyMenu();

        if (choice == 0) {
            runPlayModeMenu();
        }
        else if (choice == 1) {
            showOption();
        }
        else if (choice == 2) {
            system("cls");
            std::cout << "Exiting game pack...\n";
            break;
        }
    }
}
