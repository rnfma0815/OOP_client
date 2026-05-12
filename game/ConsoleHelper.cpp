#include "ConsoleHelper.h"
#include <iostream>

namespace ConsoleHelper {
    HANDLE getConsoleHandle() {
        return GetStdHandle(STD_OUTPUT_HANDLE);
    }

    void setTextColor(WORD color) {
        SetConsoleTextAttribute(getConsoleHandle(), color);
    }

    void clearScreen() {
        system("cls");
    }

    void setDefaultTheme() {
        system("color 0E");
    }

    void setCursorPosition(short x, short y) {
        COORD pos;
        pos.X = x;
        pos.Y = y;
        SetConsoleCursorPosition(getConsoleHandle(), pos);
    }

    void printCentered(const std::string& text, int width, int offset) {
        int padding = (width - (int)text.length()) / 2 + offset;
        if (padding < 0) padding = 0;
        std::cout << std::string(padding, ' ') << text << "\n";
    }

    void drawHorizontalMenu(const std::vector<std::string>& menu, int selected, int indent) {
        std::string space(indent, ' ');
        std::cout << space;

        for (int i = 0; i < (int)menu.size(); i++) {
            if (i == selected) {
                std::cout << "▶ " << menu[i] << " ◀";
            }
            else {
                std::cout << "  " << menu[i] << "  ";
            }

            if (i != (int)menu.size() - 1) {
                std::cout << "    ";
            }
        }
        std::cout << "\n";
    }
}
