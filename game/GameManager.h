#pragma once
#include <vector>
#include <string>
#include "Game.h"
#include "State.h"

class GameManager {
private:
    std::vector<Game*> games;
    std::vector<std::string> lobbyMenu;
    std::vector<std::string> playModeMenu;
    std::vector<std::string> onlineGameMenu;
    std::vector<std::string> gameMenu;
    State::Difficulty difficulty = State::Difficulty::EASY;

public:
    GameManager();
    ~GameManager();

    void run();

private:
    void drawLobbyMenu(int selected);
    int selectLobbyMenu();

    void drawPlayModeMenu(int selected);
    void runPlayModeMenu();

    void drawGameSelectScreen(int selectedGame, int selectedMenu);
    void runGameSelectMenu();

    void showDescription(int gameIndex);
    void showOption();
    void showOnlineMode();
    void drawOnlineGameSelectScreen(int selectedGame, int selectedMenu);
    void runOnlineGameMenu();

    std::string getDifficultyString();
    void changeDifficulty(int direction);
};
