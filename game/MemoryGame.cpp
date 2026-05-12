#include "MemoryGame.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#define NOMINMAX
#include <windows.h>
#include <conio.h>

MemoryGame::MemoryGame()
    : nextCardIndex(0),
    rows(0), cols(0),
    cursorRow(0), cursorCol(0),
    attempts(0), matchedPairs(0), totalPairs(0),
    remainingDeckCards(0), hardMode(false) {
}

void MemoryGame::play() {
    while (true) {
        setupGame();

        bool exitToMenu = false;
        bool restartGame = false;

        while (true) {
            drawBoard();

            if (isGameClear()) {
                std::cout << "\nCongratulations! You cleared the Memory Game!\n";
                std::cout << "Press any key to return...";
                _getch();
                return;
            }

            handleInput(exitToMenu, restartGame);

            if (exitToMenu) return;
            if (restartGame) break;
        }
    }
}

void MemoryGame::setupGame() {
    board.clear();
    remainingCards.clear();
    selectedCards.clear();

    nextCardIndex = 0;
    cursorRow = 0;
    cursorCol = 0;
    attempts = 0;
    matchedPairs = 0;
    totalPairs = 0;
    remainingDeckCards = 0;
    hardMode = false;

    if (difficulty == State::Difficulty::EASY) {
        setupEasy();
    }
    else if (difficulty == State::Difficulty::NORMAL) {
        setupNormal();
    }
    else {
        setupHard();
    }
}

void MemoryGame::setupEasy() {
    rows = 2;
    cols = 4;
    hardMode = false;
    totalPairs = 4;

    board.assign(rows, std::vector<Slot>(cols));
    createEasyNormalCards(totalPairs);
}

void MemoryGame::setupNormal() {
    rows = 2;
    cols = 5;
    hardMode = false;
    totalPairs = 5;

    board.assign(rows, std::vector<Slot>(cols));
    createEasyNormalCards(totalPairs);
}

void MemoryGame::setupHard() {
    rows = 2;
    cols = 6;
    hardMode = true;
    totalPairs = 26;

    board.assign(rows, std::vector<Slot>(cols));

    std::vector<std::string> ranks = {
        "A", "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "J", "Q", "K"
    };

    std::vector<std::string> suits = {
        "Spade", "Diamond", "Heart", "Clover"
    };

    for (const auto& suit : suits) {
        for (const auto& rank : ranks) {
            remainingCards.push_back(Card(rank, suit));
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(remainingCards.begin(), remainingCards.end(), g);

    nextCardIndex = 0;
    refillBoard();
}

void MemoryGame::createEasyNormalCards(int pairCount) {
    std::vector<Card> cards;
    std::vector<std::string> ranks = {
        "A", "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "J", "Q", "K"
    };

    int rankIndex = 0;

    while ((int)cards.size() < pairCount * 2) {
        std::string rank = ranks[rankIndex];

        cards.push_back(Card(rank, "Spade"));
        cards.push_back(Card(rank, "Clover"));

        if ((int)cards.size() < pairCount * 2) {
            cards.push_back(Card(rank, "Heart"));
            cards.push_back(Card(rank, "Diamond"));
        }

        rankIndex++;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);

    placeCardsToBoard(cards);
}

void MemoryGame::placeCardsToBoard(const std::vector<Card>& cards) {
    int index = 0;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            board[r][c].card = cards[index++];
            board[r][c].hasCard = true;
            board[r][c].opened = false;
            board[r][c].matched = false;
        }
    }
}

void MemoryGame::refillBoard() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (!board[r][c].hasCard && nextCardIndex < (int)remainingCards.size()) {
                board[r][c].card = remainingCards[nextCardIndex++];
                board[r][c].hasCard = true;
                board[r][c].opened = false;
                board[r][c].matched = false;
            }
        }
    }

    remainingDeckCards = (int)remainingCards.size() - nextCardIndex;
}

void MemoryGame::shuffleRemainingCards() {
    if (!hardMode) return;

    std::vector<Card> pool;

    // 현재 필드에 남아 있는 카드 회수
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (board[r][c].hasCard) {
                pool.push_back(board[r][c].card);
                board[r][c].hasCard = false;
                board[r][c].opened = false;
                board[r][c].matched = false;
            }
        }
    }

    // 아직 덱에 남아 있는 카드 회수
    for (int i = nextCardIndex; i < (int)remainingCards.size(); i++) {
        pool.push_back(remainingCards[i]);
    }

    // 다시 남은 카드 전체로 구성
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(pool.begin(), pool.end(), g);

    remainingCards = pool;
    nextCardIndex = 0;
    selectedCards.clear();

    refillBoard();

    if (!board[cursorRow][cursorCol].hasCard) {
        cursorRow = 0;
        cursorCol = 0;
    }
}

void MemoryGame::drawBoard() const {
    view.drawBoard(makeViewBoard(),
                   rows,
                   cols,
                   cursorRow,
                   cursorCol,
                   attempts,
                   matchedPairs,
                   totalPairs,
                   remainingDeckCards,
                   hardMode,
                   getDifficultyString());
}

std::string MemoryGame::getDifficultyString() const {
    if (difficulty == State::Difficulty::EASY) return "Easy";
    if (difficulty == State::Difficulty::NORMAL) return "Normal";
    return "Hard";
}

std::vector<std::vector<MemoryGameView::SlotViewData>> MemoryGame::makeViewBoard() const {
    std::vector<std::vector<MemoryGameView::SlotViewData>> viewBoard(rows, std::vector<MemoryGameView::SlotViewData>(cols));

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            viewBoard[r][c].card = board[r][c].card;
            viewBoard[r][c].hasCard = board[r][c].hasCard;
            viewBoard[r][c].opened = board[r][c].opened;
            viewBoard[r][c].matched = board[r][c].matched;
        }
    }

    return viewBoard;
}

void MemoryGame::handleInput(bool& exitToMenu, bool& restartGame) {
    int key = _getch();

    if (key == 27) {
        State::PauseResult result = showPauseMenu();

        if (result == State::PauseResult::RESUME) return;
        if (result == State::PauseResult::RESTART) {
            restartGame = true;
            return;
        }
        if (result == State::PauseResult::EXIT_TO_MENU) {
            exitToMenu = true;
            return;
        }
    }

    if (key == 224 || key == 0) {
        int arrow = _getch();

        if (arrow == 72) moveCursor(-1, 0);
        else if (arrow == 80) moveCursor(1, 0);
        else if (arrow == 75) moveCursor(0, -1);
        else if (arrow == 77) moveCursor(0, 1);
    }
    else if (key == 13) {
        selectCard(exitToMenu, restartGame);
    }
    else if ((key == 'r' || key == 'R') && hardMode) {
        shuffleRemainingCards();
    }
}

void MemoryGame::moveCursor(int dRow, int dCol) {
    int newRow = cursorRow + dRow;
    int newCol = cursorCol + dCol;

    if (newRow < 0) newRow = 0;
    if (newRow >= rows) newRow = rows - 1;
    if (newCol < 0) newCol = 0;
    if (newCol >= cols) newCol = cols - 1;

    cursorRow = newRow;
    cursorCol = newCol;
}

void MemoryGame::selectCard(bool& exitToMenu, bool& restartGame) {
    Slot& current = board[cursorRow][cursorCol];

    if (!current.hasCard) return;
    if (current.matched) return;
    if (current.opened) return;
    if (selectedCards.size() >= 2) return;

    current.opened = true;
    selectedCards.push_back({ cursorRow, cursorCol });

    if (selectedCards.size() < 2) return;

    drawBoard();
    Sleep(500);

    int r1 = selectedCards[0].first;
    int c1 = selectedCards[0].second;
    int r2 = selectedCards[1].first;
    int c2 = selectedCards[1].second;

    Slot& first = board[r1][c1];
    Slot& second = board[r2][c2];

    attempts++;

    if (isPair(first.card, second.card)) {
        matchedPairs++;

        if (hardMode) {
            Sleep(400);

            first.hasCard = false;
            second.hasCard = false;
            first.opened = false;
            second.opened = false;
            first.matched = false;
            second.matched = false;

            refillBoard();
        }
        else {
            first.matched = true;
            second.matched = true;
        }
    }
    else {
        Sleep(900);
        first.opened = false;
        second.opened = false;
    }

    selectedCards.clear();

    if (hardMode && !board[cursorRow][cursorCol].hasCard) {
        for (int r = 0; r < rows; r++) {
            bool found = false;
            for (int c = 0; c < cols; c++) {
                if (board[r][c].hasCard) {
                    cursorRow = r;
                    cursorCol = c;
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
    }
}

bool MemoryGame::isPair(const Card& a, const Card& b) const {
    return a.getRank() == b.getRank()
        && isRedSuit(a.getSuit()) == isRedSuit(b.getSuit())
        && a.getSuit() != b.getSuit();
}

bool MemoryGame::isRedSuit(const std::string& suit) const {
    return (suit == "Heart" || suit == "Diamond");
}

bool MemoryGame::isGameClear() const {
    if (hardMode) {
        if (remainingDeckCards > 0) return false;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (board[r][c].hasCard) return false;
            }
        }
        return true;
    }

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (!board[r][c].matched) return false;
        }
    }
    return true;
}