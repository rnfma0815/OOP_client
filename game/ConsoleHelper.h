#pragma once
#include <string>
#include <vector>
#include <windows.h>

namespace ConsoleHelper {
    HANDLE getConsoleHandle();
    void setTextColor(WORD color);
    void clearScreen();
    void setDefaultTheme();
    void setCursorPosition(short x, short y);
    void printCentered(const std::string& text, int width = 60, int offset = 0);
    void drawHorizontalMenu(const std::vector<std::string>& menu, int selected, int indent = 10);
}
