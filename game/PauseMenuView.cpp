#include "PauseMenuView.h"
#include "ConsoleHelper.h"
#include "InputHelper.h"
#include <iostream>
#include <vector>
#include <string>

State::PauseResult PauseMenuView::run() const {
    std::vector<std::string> pauseMenu;
    pauseMenu.push_back("Resume");
    pauseMenu.push_back("Restart");
    pauseMenu.push_back("Back to Menu");

    int selected = 0;

    while (true) {
        ConsoleHelper::clearScreen();
        ConsoleHelper::setDefaultTheme();

        std::cout << "\n\n\n\n";
        std::cout << "                ┌──────────────────────────────┐\n";
        std::cout << "                │          PAUSE MENU          │\n";
        std::cout << "                ├──────────────────────────────┤\n";

        for (int i = 0; i < (int)pauseMenu.size(); i++) {
            std::string text = (i == selected) ? "> " + pauseMenu[i] + " <" : pauseMenu[i];

            int padding = 30 - (int)text.length();
            int left = padding / 2;
            int right = padding - left;

            std::cout << "                │";
            for (int j = 0; j < left; j++) std::cout << ' ';
            std::cout << text;
            for (int j = 0; j < right; j++) std::cout << ' ';
            std::cout << "│\n";
        }

        std::cout << "                └──────────────────────────────┘\n";
        std::cout << "\n          Use UP / DOWN and press ENTER\n";

        InputHelper::Key key = InputHelper::readKey();

        if (key == InputHelper::Key::UP) {
            selected = (selected - 1 + (int)pauseMenu.size()) % (int)pauseMenu.size();
        }
        else if (key == InputHelper::Key::DOWN) {
            selected = (selected + 1) % (int)pauseMenu.size();
        }
        else if (key == InputHelper::Key::ENTER) {
            if (selected == 0) return State::PauseResult::RESUME;
            if (selected == 1) return State::PauseResult::RESTART;
            return State::PauseResult::EXIT_TO_MENU;
        }
    }
}

State::PauseResult PauseMenuView::runOnline() const {
    std::vector<std::string> pauseMenu;
    pauseMenu.push_back("Resume");
    pauseMenu.push_back("Leave Room");

    int selected = 0;

    while (true) {
        ConsoleHelper::clearScreen();
        ConsoleHelper::setDefaultTheme();

        std::cout << "\n\n\n\n";
        std::cout << "                ┌──────────────────────────────┐\n";
        std::cout << "                │          ONLINE PAUSE        │\n";
        std::cout << "                ├──────────────────────────────┤\n";

        for (int i = 0; i < (int)pauseMenu.size(); i++) {
            std::string text = (i == selected) ? "> " + pauseMenu[i] + " <" : pauseMenu[i];

            int padding = 30 - (int)text.length();
            int left = padding / 2;
            int right = padding - left;

            std::cout << "                │";
            for (int j = 0; j < left; j++) std::cout << ' ';
            std::cout << text;
            for (int j = 0; j < right; j++) std::cout << ' ';
            std::cout << "│\n";
        }

        std::cout << "                └──────────────────────────────┘\n";
        std::cout << "\n          Use UP / DOWN and press ENTER, ESC to resume\n";

        InputHelper::Key key = InputHelper::readKey();

        if (key == InputHelper::Key::UP) {
            selected = (selected - 1 + (int)pauseMenu.size()) % (int)pauseMenu.size();
        }
        else if (key == InputHelper::Key::DOWN) {
            selected = (selected + 1) % (int)pauseMenu.size();
        }
        else if (key == InputHelper::Key::ESC) {
            return State::PauseResult::RESUME;
        }
        else if (key == InputHelper::Key::ENTER) {
            if (selected == 0) return State::PauseResult::RESUME;
            return State::PauseResult::EXIT_TO_MENU;
        }
    }
}
