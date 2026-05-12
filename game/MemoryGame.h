#pragma once
#include "Game.h"
#include "Card.h"
#include "MemoryGameView.h"
#include <vector>
#include <utility>
#include <string>

class MemoryGame : public Game {
private:
    struct Slot {
        Card card;
        bool hasCard = false;
        bool opened = false;
        bool matched = false;
    };

private:
    std::vector<std::vector<Slot>> board;
    std::vector<Card> remainingCards;
    int nextCardIndex;
    int rows;
    int cols;
    int cursorRow;
    int cursorCol;
    int attempts;
    int matchedPairs;
    int totalPairs;
    int remainingDeckCards;
    bool hardMode;
    std::vector<std::pair<int, int>> selectedCards;
    MemoryGameView view;

public:
    MemoryGame();
    void play() override;

private:
    void setupGame();
    void setupEasy();
    void setupNormal();
    void setupHard();

    void createEasyNormalCards(int pairCount);
    void placeCardsToBoard(const std::vector<Card>& cards);
    void refillBoard();
    void shuffleRemainingCards();

    void drawBoard() const;
    std::string getDifficultyString() const;
    std::vector<std::vector<MemoryGameView::SlotViewData>> makeViewBoard() const;

    void handleInput(bool& exitToMenu, bool& restartGame);
    void moveCursor(int dRow, int dCol);
    void selectCard(bool& exitToMenu, bool& restartGame);

    bool isPair(const Card& a, const Card& b) const;
    bool isRedSuit(const std::string& suit) const;
    bool isGameClear() const;
};
