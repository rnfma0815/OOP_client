#pragma once
#include <vector>
#include <string>
#include "State.h"

class ClimbingGameView {
public:
    void drawGameScreen(const std::vector<std::string>& playerCardNames,
        int selected,
        int playerScore,
        int computerScore,
        int roundNumber,
        int maxRounds,
        int currentSet,
        int maxSets,
        int playerSetWins,
        int computerSetWins,
        const std::string& difficultyText) const;

    void drawGameScreen(const std::vector<std::string>& playerCardNames,
        int selected,
        int playerScore,
        int opponentScore,
        int roundNumber,
        int maxRounds,
        int currentSet,
        int maxSets,
        int playerSetWins,
        int opponentSetWins,
        const std::string& difficultyText,
        const std::string& playerLabel,
        const std::string& opponentLabel) const;

    void drawRoundResult(const std::string& playerName,
        const std::string& computerName,
        const std::string& playerArt,
        const std::string& computerArt,
        int result,
        int playerScore,
        int computerScore,
        int currentSet,
        int maxSets,
        int playerSetWins,
        int computerSetWins) const;

    void drawFinalResult(int playerScore, int computerScore) const;

    void drawMultiplayerRoundResult(const std::string& playerName,
        const std::string& opponentName,
        const std::string& playerCardName,
        const std::string& opponentCardName,
        const std::string& playerArt,
        const std::string& opponentArt,
        int result,
        int roundNumber,
        int maxRounds,
        int setNumber,
        int maxSets,
        int playerRoundScore,
        int opponentRoundScore,
        int playerSetWins,
        int opponentSetWins) const;

    std::string getRoleArt(const std::string& roleName) const;
};
