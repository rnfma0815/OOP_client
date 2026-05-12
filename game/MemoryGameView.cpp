#include "MemoryGameView.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <windows.h>

static void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

bool MemoryGameView::isRedSuit(const std::string& suit) const {
    return (suit == "Heart" || suit == "Diamond");
}

void MemoryGameView::drawBoard(const std::vector<std::vector<SlotViewData>>& board,
                               int rows,
                               int cols,
                               int cursorRow,
                               int cursorCol,
                               int attempts,
                               int matchedPairs,
                               int totalPairs,
                               int remainingDeckCards,
                               bool hardMode,
                               const std::string& difficultyText) const {
    ConsoleHelper::clearScreen();
    ConsoleHelper::setDefaultTheme();

    std::cout << "================ MEMORY GAME ================\n\n";
    std::cout << "Difficulty : " << difficultyText << "\n";
    std::cout << "Attempts   : " << attempts << "\n";
    std::cout << "Matched    : " << matchedPairs << " / " << totalPairs << "\n";

    if (hardMode) {
        std::cout << "Deck Left  : " << remainingDeckCards << "\n";
    }

    std::cout << "\n";

    for (int r = 0; r < rows; r++) {
        for (int line = 0; line < 7; line++) {
            for (int c = 0; c < cols; c++) {
                bool isCursor = (r == cursorRow && c == cursorCol);
                drawCardCell(board[r][c], isCursor, line);
                std::cout << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    if (hardMode) {
        std::cout << "Use ARROW KEYS to move, ENTER to flip, R to shuffle, ESC to pause\n";
    }
    else {
        std::cout << "Use ARROW KEYS to move, ENTER to flip, ESC to pause\n";
    }

    std::cout << "Pair Rule: Same rank + same color\n";
    std::cout << "Example: ♠A <-> ♣A,   ♥7 <-> ♦7\n";
}

void MemoryGameView::drawCardCell(const SlotViewData& slot, bool isCursor, int line) const {
    if (!slot.hasCard) {
        std::cout << "         ";
        return;
    }

    std::string top = isCursor ? "┏━━━━━━━┓" : "┌───────┐";
    std::string midL = isCursor ? "┃" : "│";
    std::string midR = isCursor ? "┃" : "│";
    std::string bottom = isCursor ? "┗━━━━━━━┛" : "└───────┘";

    int borderColor = isCursor ? 11 : 14;

    if (!(slot.opened || slot.matched)) {
        setConsoleColor(borderColor);

        if (line == 0) std::cout << top;
        else if (line == 1) std::cout << midL << "  ???  " << midR;
        else if (line == 2) std::cout << midL << " ?   ? " << midR;
        else if (line == 3) std::cout << midL << "    ?  " << midR;
        else if (line == 4) std::cout << midL << "   ?   " << midR;
        else if (line == 5) std::cout << midL << "   ?   " << midR;
        else if (line == 6) std::cout << bottom;

        setConsoleColor(14);
        return;
    }

    std::string rank = slot.card.getRank();
    std::string suit = slot.card.getSuitSymbol();
    int textColor = isRedSuit(slot.card.getSuit()) ? 12 : 15;

    if (line == 0) {
        setConsoleColor(borderColor);
        std::cout << top;
        setConsoleColor(14);
    }
    else if (line == 1) {
        setConsoleColor(borderColor);
        std::cout << midL;
        setConsoleColor(textColor);

        if (rank.length() == 2)
            std::cout << rank << "     ";
        else
            std::cout << rank << "      ";

        setConsoleColor(borderColor);
        std::cout << midR;
        setConsoleColor(14);
    }
    else if (line == 2) {
        setConsoleColor(borderColor);
        std::cout << midL << "       " << midR;
        setConsoleColor(14);
    }
    else if (line == 3) {
        setConsoleColor(borderColor);
        std::cout << midL;
        setConsoleColor(textColor);
        std::cout << "   " << suit << "   ";
        setConsoleColor(borderColor);
        std::cout << midR;
        setConsoleColor(14);
    }
    else if (line == 4) {
        setConsoleColor(borderColor);
        std::cout << midL << "       " << midR;
        setConsoleColor(14);
    }
    else if (line == 5) {
        setConsoleColor(borderColor);
        std::cout << midL;
        setConsoleColor(textColor);

        if (rank.length() == 2)
            std::cout << "     " << rank;
        else
            std::cout << "      " << rank;

        setConsoleColor(borderColor);
        std::cout << midR;
        setConsoleColor(14);
    }
    else if (line == 6) {
        setConsoleColor(borderColor);
        std::cout << bottom;
        setConsoleColor(14);
    }
}
