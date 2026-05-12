#include "BlackjackView.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <windows.h>

void BlackjackView::drawWinArt() const {
    ConsoleHelper::printCentered("__        __      ___       _   _ ", 60, -2);
    ConsoleHelper::printCentered("\\ \\      / /     |_ _|     | \\ | |", 60, -2);
    ConsoleHelper::printCentered(" \\ \\ /\\ / /       | |      |  \\\\| |", 60, -2);
    ConsoleHelper::printCentered("  \\ V  V /        | |      | |\\  |", 60, -2);
    ConsoleHelper::printCentered("   \\_/\\_/        |___|     |_| \\_|", 60, -2);
}

void BlackjackView::drawLoseArt() const {
    ConsoleHelper::printCentered(" _          ___      ____      _____ ", 60, -4);
    ConsoleHelper::printCentered("| |        / _ \\    / ___|    | ____|", 60, -4);
    ConsoleHelper::printCentered("| |       | | | |   \\___ \\    |  _|  ", 60, -4);
    ConsoleHelper::printCentered("| |___    | |_| |    ___) |   | |___ ", 60, -4);
    ConsoleHelper::printCentered("|_____|    \\___/    |____/    |_____|", 60, -4);
}

void BlackjackView::drawDrawArt() const {
    ConsoleHelper::printCentered(" ____      ____       _      __        __", 60, -3);
    ConsoleHelper::printCentered("|  _ \\    |  _ \\     / \\     \\ \\      / /", 60, -3);
    ConsoleHelper::printCentered("| | | |   | |_) |   / _ \\     \\ \\ /\\ / / ", 60, -3);
    ConsoleHelper::printCentered("| |_| |   |  _ <   / ___ \\     \\ V  V /  ", 60, -3);
    ConsoleHelper::printCentered("|____/    |_| \\_\\ /_/   \\_\\     \\_/\\_/   ", 60, -3);
}

void BlackjackView::drawFinalResultBase(int playerPoint, int dealerPoint) const {
    ConsoleHelper::setTextColor(14);

    std::cout << "==================================================\n";
    ConsoleHelper::printCentered("FINAL RESULT", 50);
    std::cout << "==================================================\n\n";

    for (int i = 0; i < 5; i++) {
        std::cout << "\n";
    }

    std::cout << "\n";
    ConsoleHelper::printCentered("Player Point : " + std::to_string(playerPoint), 50);
    ConsoleHelper::printCentered("Dealer Point : " + std::to_string(dealerPoint), 50);

    std::cout << "\n--------------------------------------------------\n\n";
    ConsoleHelper::printCentered("Press any key to return...", 50);
}

void BlackjackView::drawFinalResultArt(int playerPoint, int dealerPoint, WORD color) const {
    ConsoleHelper::setTextColor(color);

    if (playerPoint > dealerPoint) {
        drawWinArt();
    }
    else if (playerPoint < dealerPoint) {
        drawLoseArt();
    }
    else {
        drawDrawArt();
    }
}

void BlackjackView::drawHugePlayerField(const std::vector<Card>& openedCards,
                                        const std::vector<Card>& roundCards) const {
    int openedCount = (int)openedCards.size();
    int closedCount = (int)roundCards.size();
    int totalCount = openedCount + closedCount;

    if (totalCount == 0) {
        std::cout << "                        (No cards)\n";
        return;
    }

    std::string indent = " ";

    std::cout << indent;
    for (int i = 0; i < totalCount; i++) {
        std::cout << "┌─────────┐  ";
    }
    std::cout << "\n";

    std::cout << indent;
    for (int i = 0; i < totalCount; i++) {
        if (i < openedCount) {
            std::string rank = openedCards[i].getRank();
            if (rank.size() == 1) rank += " ";
            std::cout << "│" << rank << "       │  ";
        }
        else {
            std::cout << "│   ???   │  ";
        }
    }
    std::cout << "\n";

    std::cout << indent;
    for (int i = 0; i < totalCount; i++) {
        if (i < openedCount) {
            std::cout << "│         │  ";
        }
        else {
            std::cout << "│  ?   ?  │  ";
        }
    }
    std::cout << "\n";

    std::cout << indent;
    for (int i = 0; i < totalCount; i++) {
        if (i < openedCount) {
            std::string suit = openedCards[i].getSuitSymbol();
            std::cout << "│    " << suit << "    │  ";
        }
        else {
            std::cout << "│     ?   │  ";
        }
    }
    std::cout << "\n";

    std::cout << indent;
    for (int i = 0; i < totalCount; i++) {
        if (i < openedCount) {
            std::cout << "│         │  ";
        }
        else {
            std::cout << "│    ?    │  ";
        }
    }
    std::cout << "\n";

    std::cout << indent;
    for (int i = 0; i < totalCount; i++) {
        if (i < openedCount) {
            std::string rank = openedCards[i].getRank();
            if (rank.size() == 1) rank = " " + rank;
            std::cout << "│       " << rank << "│  ";
        }
        else {
            std::cout << "│    ○    │  ";
        }
    }
    std::cout << "\n";

    std::cout << indent;
    for (int i = 0; i < totalCount; i++) {
        std::cout << "└─────────┘  ";
    }
    std::cout << "\n";
}

void BlackjackView::drawGameScreen(const BlackjackPlayer& player,
                                   const BlackjackDealer& dealer,
                                   int currentRound,
                                   int selectedMenu) const {
    ConsoleHelper::clearScreen();
    ConsoleHelper::setDefaultTheme();

    std::cout << "============================================================\n";
    std::cout << "                         BLACKJACK\n";
    std::cout << "============================================================\n";
    std::cout << "Round " << currentRound << " / 5";
    std::cout << "            [ Field ]";
    std::cout << "           Player: " << player.getWinPoint()
        << "  Dealer: " << dealer.getWinPoint() << "\n\n\n";

    drawHugePlayerField(player.getOpenedCards(), player.getRoundCards());
    std::cout << "\n";

    std::cout << "                          Score : " << player.calculateScore() << "\n\n";

    std::cout << "Hidden Cards : ";
    const std::vector<Card>& hidden = player.getHiddenCards();

    if (hidden.empty()) {
        std::cout << "(None)";
    }
    else {
        for (const Card& card : hidden) {
            std::cout << "[" << card.toString() << "] ";
        }
    }
    std::cout << "\n\n";

    std::vector<std::string> menu = { "Open", "Stop", "Use Hidden" };
    ConsoleHelper::drawHorizontalMenu(menu, selectedMenu, 17);

    std::cout << "\n";
    std::cout << "                 LEFT / RIGHT : Menu   ENTER : Select\n";
}

void BlackjackView::drawMultiplayerGameScreen(const std::string& playerName,
                                              const std::string& opponentName,
                                              const std::vector<Card>& playerCards,
                                              const std::vector<Card>& hiddenCards,
                                              const std::vector<Card>& dealerCards,
                                              const std::string& opponentFirstCard,
                                              int currentRound,
                                              int maxRounds,
                                              int playerPoint,
                                              int opponentPoint,
                                              int selectedMenu) const {
    ConsoleHelper::clearScreen();
    ConsoleHelper::setDefaultTheme();

    int score = 0;
    int aceCount = 0;
    for (const Card& card : playerCards) {
        score += card.getValue();
        if (card.getRank() == "A") aceCount++;
    }
    while (score > 21 && aceCount > 0) {
        score -= 10;
        aceCount--;
    }

    std::cout << "============================================================\n";
    std::cout << "                    ONLINE BLACKJACK\n";
    std::cout << "============================================================\n";
    std::cout << "Round " << currentRound << " / " << maxRounds;
    std::cout << "            [ Field ]";
    std::cout << "           " << playerName << ": " << playerPoint
              << "  " << opponentName << ": " << opponentPoint << "\n\n";

    std::cout << "Dealer Open Card\n";
    drawHugePlayerField(dealerCards, {});
    std::cout << "\n";

    std::cout << opponentName << " First Card : " << opponentFirstCard << "\n\n";

    std::cout << playerName << " Cards\n";
    drawHugePlayerField(playerCards, {});
    std::cout << "\n";

    std::cout << "                          Score : " << score << "\n\n";

    std::cout << "Hidden Cards : ";
    if (hiddenCards.empty()) {
        std::cout << "(None)";
    }
    else {
        for (const Card& card : hiddenCards) {
            std::cout << "[" << card.toString() << "] ";
        }
    }
    std::cout << "\n\n";

    std::vector<std::string> menu = { "Open", "Stop", "Use Hidden" };
    ConsoleHelper::drawHorizontalMenu(menu, selectedMenu, 17);

    std::cout << "\n";
    std::cout << "          LEFT / RIGHT : Menu   ENTER : Select   ESC : Leave\n";
}

void BlackjackView::showMultiplayerRoundResult(const std::string& playerName,
                                               const std::string& opponentName,
                                               const std::vector<Card>& playerCards,
                                               const std::vector<Card>& opponentCards,
                                               const std::vector<Card>& dealerCards,
                                               const std::string& playerResult,
                                               const std::string& opponentResult,
                                               int currentRound,
                                               int maxRounds,
                                               int playerPoint,
                                               int opponentPoint) const {
    ConsoleHelper::clearScreen();
    ConsoleHelper::setDefaultTheme();

    std::cout << "============================================================\n";
    std::cout << "                    BLACKJACK ROUND RESULT\n";
    std::cout << "============================================================\n\n";

    std::cout << "Round " << currentRound << " / " << maxRounds << "\n";
    std::cout << "Match Score : " << playerName << " " << playerPoint
              << "  -  " << opponentName << " " << opponentPoint << "\n\n";

    std::cout << "Dealer Cards\n";
    drawHugePlayerField(dealerCards, {});
    std::cout << "\n";

    std::cout << playerName << " Cards  [" << playerResult << "]\n";
    drawHugePlayerField(playerCards, {});
    std::cout << "\n";

    std::cout << opponentName << " Cards  [" << opponentResult << "]\n";
    drawHugePlayerField(opponentCards, {});

    std::cout << "\nPress any key to continue...";
    _getch();
}

void BlackjackView::drawHiddenSelectScreen(const BlackjackPlayer& player,
                                           int selectedHidden) const {
    ConsoleHelper::clearScreen();
    ConsoleHelper::setDefaultTheme();

    std::cout << "============================================================\n";
    std::cout << "                      USE HIDDEN CARD\n";
    std::cout << "============================================================\n\n";

    const std::vector<Card>& hidden = player.getHiddenCards();

    if (hidden.empty()) {
        std::cout << "                    No hidden cards left.\n";
        std::cout << "\n                Press any key to return...";
        _getch();
        return;
    }

    std::cout << "                       Hidden Cards\n\n";

    if (hidden.size() == 1) {
        std::string text = "[" + hidden[0].toString() + "]";
        if (selectedHidden == 0) {
            std::cout << "                    ▶ " << text << " ◀\n";
        }
        else {
            std::cout << "                      " << text << "\n";
        }
    }
    else {
        std::string leftText = "[" + hidden[0].toString() + "]";
        std::string rightText = "[" + hidden[1].toString() + "]";

        std::cout << "              ";
        if (selectedHidden == 0) std::cout << "▶ " << leftText << " ◀";
        else std::cout << "  " << leftText << "  ";

        std::cout << "        ";

        if (selectedHidden == 1) std::cout << "▶ " << rightText << " ◀";
        else std::cout << "  " << rightText << "  ";

        std::cout << "\n";
    }

    std::cout << "\n";
    std::cout << "              LEFT / RIGHT : Choose hidden card\n";
    std::cout << "              ENTER : Use    ESC : Back\n";
}

void BlackjackView::showRoundResult(const BlackjackPlayer& player,
                                    const BlackjackDealer& dealer,
                                    int currentRound) const {
    ConsoleHelper::clearScreen();
    ConsoleHelper::setDefaultTheme();

    int playerScore = player.calculateScore();
    int dealerScore = dealer.calculateScore();

    std::cout << "============================================================\n";
    std::cout << "                        ROUND RESULT\n";
    std::cout << "============================================================\n\n";

    std::cout << "Round " << currentRound << " finished.\n\n";

    std::cout << "Dealer Opened Cards : ";
    for (const Card& card : dealer.getOpenedCards()) {
        std::cout << "[" << card.toString() << "] ";
    }
    std::cout << "\n";

    std::cout << "Dealer Closed Cards : ";
    for (const Card& card : dealer.getRoundCards()) {
        std::cout << "[" << card.toString() << "] ";
    }
    if (dealer.getRoundCards().empty()) {
        std::cout << "(None)";
    }
    std::cout << "\n";

    std::cout << "Dealer Score : " << dealerScore << "\n\n";

    std::cout << "Player Opened Cards : ";
    for (const Card& card : player.getOpenedCards()) {
        std::cout << "[" << card.toString() << "] ";
    }
    std::cout << "\n";

    std::cout << "Player Closed Cards : ";
    for (const Card& card : player.getRoundCards()) {
        std::cout << "[" << card.toString() << "] ";
    }
    if (player.getRoundCards().empty()) {
        std::cout << "(None)";
    }
    std::cout << "\n";

    if (player.getHiddenCards().empty()) {
        std::cout << "(None)";
    }
    std::cout << "\n";

    std::cout << "Player Score : " << playerScore << "\n\n";

    std::cout << "Player Point : " << player.getWinPoint() << "\n";
    std::cout << "Dealer Point : " << dealer.getWinPoint() << "\n\n";

    if (playerScore == 21) {
        std::cout << "Player made exactly 21. Player wins this round.\n";
    }
    else if (playerScore > 21) {
        std::cout << "Player exceeded 21. Dealer wins this round.\n";
    }
    else if (dealerScore > 21) {
        std::cout << "Dealer exceeded 21. Player wins this round.\n";
    }
    else {
        int playerDiff = 21 - playerScore;
        int dealerDiff = 21 - dealerScore;

        if (playerDiff < dealerDiff) {
            std::cout << "Player is closer to 21. Player wins this round.\n";
        }
        else if (dealerDiff < playerDiff) {
            std::cout << "Dealer is closer to 21. Dealer wins this round.\n";
        }
        else {
            std::cout << "Deuce. Both are equally close to 21.\n";
        }
    }

    std::cout << "\nPress any key to continue...";
    _getch();
}

void BlackjackView::showFinalResult(int playerPoint, int dealerPoint) const {
    while (true) {
        if (_kbhit()) {
            _getch();
            break;
        }

        ConsoleHelper::clearScreen();
        drawFinalResultBase(playerPoint, dealerPoint);
        ConsoleHelper::setCursorPosition(0, 3);

        if (playerPoint > dealerPoint) {
            drawFinalResultArt(playerPoint, dealerPoint, 10);
        }
        else if (playerPoint < dealerPoint) {
            drawFinalResultArt(playerPoint, dealerPoint, 12);
        }
        else {
            drawFinalResultArt(playerPoint, dealerPoint, 11);
        }

        Sleep(250);

        if (_kbhit()) {
            _getch();
            break;
        }

        ConsoleHelper::clearScreen();
        drawFinalResultBase(playerPoint, dealerPoint);
        ConsoleHelper::setCursorPosition(0, 3);
        drawFinalResultArt(playerPoint, dealerPoint, 15);
        Sleep(250);
    }

    ConsoleHelper::setTextColor(15);
    ConsoleHelper::clearScreen();
}
