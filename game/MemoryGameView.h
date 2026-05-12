#pragma once
#include "Card.h"
#include <vector>
#include <string>

class MemoryGameView {
public:
    struct SlotViewData {
        Card card;
        bool hasCard = false;
        bool opened = false;
        bool matched = false;
    };

    void drawBoard(const std::vector<std::vector<SlotViewData>>& board,
                   int rows,
                   int cols,
                   int cursorRow,
                   int cursorCol,
                   int attempts,
                   int matchedPairs,
                   int totalPairs,
                   int remainingDeckCards,
                   bool hardMode,
                   const std::string& difficultyText) const;

private:
    void drawCardCell(const SlotViewData& slot, bool isCursor, int line) const;
    bool isRedSuit(const std::string& suit) const;
};
