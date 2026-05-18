#pragma once
#include "BlackjackPlayer.h"
#include "BlackjackDealer.h"
#include <string>
#include <vector>
#include <windows.h>

class BlackjackView {
public:
    void drawGameScreen(const BlackjackPlayer& player,
                        const BlackjackDealer& dealer,
                        int currentRound,
                        int selectedMenu) const;

    void drawHiddenSelectScreen(const BlackjackPlayer& player,
                                int selectedHidden) const;

    void showRoundResult(const BlackjackPlayer& player,
                         const BlackjackDealer& dealer,
                         int currentRound) const;

    void showFinalResult(int playerPoint, int dealerPoint) const;

    void drawMultiplayerGameScreen(const std::string& playerName,
                                   const std::string& opponentName,
                                   const std::vector<Card>& playerCards,
                                   const std::vector<Card>& hiddenCards,
                                   const std::vector<Card>& dealerCards,
                                   const std::string& opponentFirstCard,
                                   int currentRound,
                                   int maxRounds,
                                   int playerPoint,
                                   int opponentPoint,
                                   int selectedMenu,
                                   const std::vector<std::string>& actionLogs = {}) const;

    void showMultiplayerRoundResult(const std::string& playerName,
                                    const std::string& opponentName,
                                    const std::vector<Card>& playerCards,
                                    const std::vector<Card>& opponentCards,
                                    const std::vector<Card>& dealerCards,
                                    const std::string& playerResult,
                                    const std::string& opponentResult,
                                    int currentRound,
                                    int maxRounds,
                                    int playerPoint,
                                    int opponentPoint) const;

private:
    void drawHugePlayerField(const std::vector<Card>& openedCards,
                             const std::vector<Card>& roundCards) const;

    void drawFinalResultBase(int playerPoint, int dealerPoint) const;
    void drawFinalResultArt(int playerPoint, int dealerPoint, WORD color) const;
    void drawWinArt() const;
    void drawLoseArt() const;
    void drawDrawArt() const;
};
