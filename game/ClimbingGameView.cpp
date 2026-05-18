#include "ClimbingGameView.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <conio.h>
#include <sstream>

namespace {
std::vector<std::string> splitArtLines(const std::string& art);
void drawCardVersusRow(const std::string& leftArt, const std::string& rightArt);
}

void ClimbingGameView::drawGameScreen(const std::vector<std::string>& playerCardNames,
    int selected,
    int playerScore,
    int computerScore,
    int roundNumber,
    int maxRounds,
    int currentSet,
    int maxSets,
    int playerSetWins,
    int computerSetWins,
    const std::string& difficultyText) const {
    drawGameScreen(playerCardNames,
        selected,
        playerScore,
        computerScore,
        roundNumber,
        maxRounds,
        currentSet,
        maxSets,
        playerSetWins,
        computerSetWins,
        difficultyText,
        "Player",
        "Computer");
}

void ClimbingGameView::drawGameScreen(const std::vector<std::string>& playerCardNames,
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
    const std::string& opponentLabel,
    const std::vector<std::string>& actionLogs) const {
    ConsoleHelper::clearScreen();
    ConsoleHelper::setDefaultTheme();

    std::cout << "============================================================\n";
    std::cout << "                       CLIMBING GAME\n";
    std::cout << "============================================================\n\n";

    std::cout << "Difficulty   : " << difficultyText << "\n";
    std::cout << "Set          : " << currentSet << " / " << maxSets << "\n";
    std::cout << "Round        : " << roundNumber << " / " << maxRounds << "\n";
    std::cout << "Set Score    : " << playerLabel << " " << playerScore
              << "  -  " << opponentLabel << " " << opponentScore << "\n";
    std::cout << "Match Score  : " << playerLabel << " " << playerSetWins
              << "  -  " << opponentLabel << " " << opponentSetWins << "\n\n";

    std::cout << "Rule\n";
    std::cout << "King > Queen, Servant\n";
    std::cout << "Queen > Servant, Joker\n";
    std::cout << "Servant > Joker\n";
    std::cout << "Joker > King\n";
    std::cout << "Same card = Draw\n\n";

    std::cout << "Your Cards\n\n";

    for (int i = 0; i < (int)playerCardNames.size(); i++) {
        if (i == selected) {
            std::cout << "                 ▶ [" << i + 1 << "] " << playerCardNames[i] << " ◀\n";
        }
        else {
            std::cout << "                   [" << i + 1 << "] " << playerCardNames[i] << "\n";
        }
    }

    if (difficultyText == "Online") {
        if (!actionLogs.empty()) {
            std::cout << "\n" << actionLogs.back() << "\n";
        }
        std::cout << "\nUse UP / DOWN to choose card, ENTER to play, ESC to leave room\n";
    }
    else {
        std::cout << "\nUse UP / DOWN to choose card, ENTER to play, ESC to pause\n";
    }
}

void ClimbingGameView::drawRoundResult(const std::string& playerName,
    const std::string& computerName,
    const std::string& playerArt,
    const std::string& computerArt,
    int result,
    int playerScore,
    int computerScore,
    int currentSet,
    int maxSets,
    int playerSetWins,
    int computerSetWins) const {
    ConsoleHelper::clearScreen();
    ConsoleHelper::setDefaultTheme();

    std::cout << "============================================================\n";
    std::cout << "                        ROUND RESULT\n";
    std::cout << "============================================================\n\n";

    std::cout << "Set         : " << currentSet << " / " << maxSets << "\n";
    std::cout << "Match Score : ME " << playerSetWins << "  -  DEALER " << computerSetWins << "\n\n";

    std::cout << "ME: " << playerName
              << "                      DEALER: " << computerName << "\n\n";

    drawCardVersusRow(playerArt, computerArt);
    std::cout << "\n";

    if (result == 1) {
        std::cout << "YOU WIN this round!\n";
    }
    else if (result == -1) {
        std::cout << "YOU LOSE this round.\n";
    }
    else {
        std::cout << "DRAW round.\n";
    }

    std::cout << "\nCurrent Set Score : ME " << playerScore
        << "  -  DEALER " << computerScore << "\n";

    std::cout << "\nPress any key to continue...";
    _getch();
}

namespace {
std::vector<std::string> splitArtLines(const std::string& art) {
    std::vector<std::string> lines;
    std::stringstream ss(art);
    std::string line;

    while (std::getline(ss, line)) {
        lines.push_back(line);
    }

    return lines;
}

void drawCardVersusRow(const std::string& leftArt, const std::string& rightArt) {
    std::vector<std::string> leftLines = splitArtLines(leftArt);
    std::vector<std::string> rightLines = splitArtLines(rightArt);
    size_t maxLines = leftLines.size() > rightLines.size() ? leftLines.size() : rightLines.size();

    for (size_t i = 0; i < maxLines; i++) {
        std::string left = i < leftLines.size() ? leftLines[i] : "";
        std::string right = i < rightLines.size() ? rightLines[i] : "";

        std::cout << left;
        if (i == maxLines / 2) {
            std::cout << "          VS          ";
        }
        else {
            std::cout << "                      ";
        }
        std::cout << right << "\n";
    }
}
}

void ClimbingGameView::drawFinalResult(int playerScore, int computerScore) const {
    ConsoleHelper::clearScreen();
    ConsoleHelper::setDefaultTheme();

    std::cout << "============================================================\n";
    std::cout << "                        FINAL RESULT\n";
    std::cout << "============================================================\n\n";

    std::cout << "Final Score : Player " << playerScore
        << "  -  Computer " << computerScore << "\n\n";

    if (playerScore > computerScore) {
        std::cout << "                 __        __  ___   _   _ \n";
        std::cout << "                 \\ \\      / / |_ _| | \\ | |\n";
        std::cout << "                  \\ \\ /\\ / /   | |  |  \\| |\n";
        std::cout << "                   \\ V  V /    | |  | . ` |\n";
        std::cout << "                    \\_/\\_/    |___| |_|\\_|\n\n";
    }
    else if (playerScore < computerScore) {
        std::cout << "                 _      _____   _____  _____ \n";
        std::cout << "                | |    /  _  \\ /  ___||  ___|\n";
        std::cout << "                | |    | | | |\\ `--. | |_   \n";
        std::cout << "                | |___ | |_| | `--. \\|  _|  \n";
        std::cout << "                |_____|\\_____/\\_____/|_|    \n\n";
    }
    else {
        std::cout << "                     _____   _____   ___   _    _ \n";
        std::cout << "                    |  _  | |  _  | / _ \\ | |  | |\n";
        std::cout << "                    | | | | | | | || /_\\ || |  | |\n";
        std::cout << "                    | |/ /  | |/ / |  _  || |/\\| |\n";
        std::cout << "                    |___/   |___/  |_| |_||__/\\__|\n\n";
    }

    std::cout << "\nPress any key to return...";
    _getch();
}

void ClimbingGameView::drawMultiplayerRoundResult(const std::string& playerName,
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
    int opponentSetWins) const {
    ConsoleHelper::clearScreen();
    ConsoleHelper::setDefaultTheme();

    std::cout << "============================================================\n";
    std::cout << "                        ROUND RESULT\n";
    std::cout << "============================================================\n\n";

    std::cout << "Set         : " << setNumber << " / " << maxSets << "\n";
    std::cout << "Round       : " << roundNumber << " / " << maxRounds << "\n";
    std::cout << "Set Score   : ME " << playerRoundScore
              << "  -  OPPONENT " << opponentRoundScore << "\n";
    std::cout << "Match Score : ME " << playerSetWins
              << "  -  OPPONENT " << opponentSetWins << "\n\n";

    std::cout << "ME: " << playerCardName
              << "                      OPPONENT: " << opponentCardName << "\n\n";

    drawCardVersusRow(playerArt, opponentArt);
    std::cout << "\n";

    if (result == 1) {
        std::cout << "YOU WIN this round!\n";
    }
    else if (result == -1) {
        std::cout << "YOU LOSE this round.\n";
    }
    else {
        std::cout << "DRAW round.\n";
    }

    std::cout << "\nPress any key to return...";
    _getch();
}

std::string ClimbingGameView::getRoleArt(const std::string& roleName) const {
    if (roleName == "King") {
        return
            "┌─────────┐\n"
            "│K        │\n"
            "│   /\\    │\n"
            "│  /  \\   │\n"
            "│  \\__/   │\n"
            "│        K│\n"
            "└─────────┘";
    }
    else if (roleName == "Queen") {
        return
            "┌─────────┐\n"
            "│Q        │\n"
            "│  .--.   │\n"
            "│ ( \\/ )  │\n"
            "│  \\__/   │\n"
            "│        Q│\n"
            "└─────────┘";
    }
    else if (roleName == "Servant") {
        return
            "┌─────────┐\n"
            "│J        │\n"
            "│   !!    │\n"
            "│   !!    │\n"
            "│   !!    │\n"
            "│        J│\n"
            "└─────────┘";
    }
    else {
        return
            "┌─────────┐\n"
            "│JKR      │\n"
            "│   ★     │\n"
            "│  \\|/    │\n"
            "│   |     │\n"
            "│      JKR│\n"
            "└─────────┘";
    }
}
