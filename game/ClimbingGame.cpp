#include "ClimbingGame.h"
#include "InputHelper.h"
#include <cstdlib>
#include <ctime>
#include <vector>

ClimbingGame::ClimbingGame()
    : playerScore(0), computerScore(0), roundNumber(1), maxRounds(4),
    currentSet(1), maxSets(3), playerSetWins(0), computerSetWins(0) {
    std::srand((unsigned)std::time(nullptr));
}

void ClimbingGame::setupMatch() {
    currentSet = 1;
    maxSets = 3;
    playerSetWins = 0;
    computerSetWins = 0;

    for (int i = 0; i < 4; i++) {
        playerHistoryByRound[i].clear();
    }

    currentSetPlayerHistory.clear();
}

void ClimbingGame::setupGame() {
    playerCards.clear();
    computerCards.clear();

    playerCards.push_back(Role::KING);
    playerCards.push_back(Role::QUEEN);
    playerCards.push_back(Role::SERVANT);
    playerCards.push_back(Role::JOKER);

    computerCards.push_back(Role::KING);
    computerCards.push_back(Role::QUEEN);
    computerCards.push_back(Role::SERVANT);
    computerCards.push_back(Role::JOKER);

    playerScore = 0;
    computerScore = 0;
    roundNumber = 1;
    maxRounds = 4;

    currentSetPlayerHistory.clear();
}

std::string ClimbingGame::getDifficultyString() const {
    if (difficulty == State::Difficulty::EASY) return "Easy";
    if (difficulty == State::Difficulty::NORMAL) return "Normal";
    return "Hard";
}

std::string ClimbingGame::roleToString(Role role) const {
    if (role == Role::KING) return "King";
    if (role == Role::QUEEN) return "Queen";
    if (role == Role::SERVANT) return "Servant";
    return "Joker";
}

std::string ClimbingGame::roleToArt(Role role) const {
    if (role == Role::KING) {
        return
            "┌─────────┐\n"
            "│K        │\n"
            "│   /\\    │\n"
            "│  /  \\   │\n"
            "│  \\__/   │\n"
            "│        K│\n"
            "└─────────┘";
    }
    else if (role == Role::QUEEN) {
        return
            "┌─────────┐\n"
            "│Q        │\n"
            "│  .--.   │\n"
            "│ ( \\/ )  │\n"
            "│  \\__/   │\n"
            "│        Q│\n"
            "└─────────┘";
    }
    else if (role == Role::SERVANT) {
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

int ClimbingGame::selectPlayerCard() {
    int selected = 0;

    while (true) {
        std::vector<std::string> playerCardNames;
        for (Role role : playerCards) {
            playerCardNames.push_back(roleToString(role));
        }

        view.drawGameScreen(playerCardNames,
            selected,
            playerScore,
            computerScore,
            roundNumber,
            maxRounds,
            currentSet,
            maxSets,
            playerSetWins,
            computerSetWins,
            getDifficultyString());

        InputHelper::Key key = InputHelper::readKey();

        if (key == InputHelper::Key::ESC) {
            State::PauseResult result = showPauseMenu();

            if (result == State::PauseResult::RESUME) {
                continue;
            }
            else if (result == State::PauseResult::RESTART) {
                return -2;
            }
            else if (result == State::PauseResult::EXIT_TO_MENU) {
                return -3;
            }
        }

        if (key == InputHelper::Key::UP) {
            selected = (selected - 1 + (int)playerCards.size()) % (int)playerCards.size();
        }
        else if (key == InputHelper::Key::DOWN) {
            selected = (selected + 1) % (int)playerCards.size();
        }
        else if (key == InputHelper::Key::ENTER) {
            return selected;
        }
    }
}

int ClimbingGame::getRandomChoice(const std::vector<Role>& cards) const {
    if (cards.empty()) return -1;
    return std::rand() % cards.size();
}

int ClimbingGame::countImmediateWins(Role computerCard, const std::vector<Role>& playerState) const {
    int winCount = 0;

    for (Role playerCard : playerState) {
        if (judgeRound(playerCard, computerCard) == -1) {
            winCount++;
        }
    }

    return winCount;
}

int ClimbingGame::countImmediateDraws(Role computerCard, const std::vector<Role>& playerState) const {
    int drawCount = 0;

    for (Role playerCard : playerState) {
        if (judgeRound(playerCard, computerCard) == 0) {
            drawCount++;
        }
    }

    return drawCount;
}

int ClimbingGame::getBestImmediateChoice() const {
    if (computerCards.empty()) return -1;

    int bestIndex = 0;
    int bestWinCount = -1;
    int bestDrawCount = -1;

    for (int i = 0; i < (int)computerCards.size(); i++) {
        int winCount = countImmediateWins(computerCards[i], playerCards);
        int drawCount = countImmediateDraws(computerCards[i], playerCards);

        if (winCount > bestWinCount) {
            bestWinCount = winCount;
            bestDrawCount = drawCount;
            bestIndex = i;
        }
        else if (winCount == bestWinCount && drawCount > bestDrawCount) {
            bestDrawCount = drawCount;
            bestIndex = i;
        }
    }

    return bestIndex;
}

double ClimbingGame::getPatternWeight(Role playerCard, int depth) const {
    // depth 0 = 현재 판의 1라운드 예상
    // depth 1 = 현재 판의 2라운드 예상
    // depth 2 = 현재 판의 3라운드 예상
    // depth 3 = 현재 판의 4라운드 예상

    if (depth < 0 || depth >= 4) return 1.0;

    // 1판은 학습 없음
    if (currentSet <= 1) {
        return 1.0;
    }

    const std::vector<Role>& history = playerHistoryByRound[depth];
    if (history.empty()) {
        return 1.0;
    }

    int sameCount = 0;
    for (Role r : history) {
        if (r == playerCard) {
            sameCount++;
        }
    }

    double baseWeight = 1.0;
    double ratio = (double)sameCount / (double)history.size();

    // 2판은 약하게 반영, 3판은 조금 더 강하게 반영
    if (currentSet == 2) {
        return baseWeight + ratio * 0.6;
    }

    // currentSet == 3
    return baseWeight + ratio * 1.0;
}

bool ClimbingGame::isBetterHardOutcome(const MatchOutcome& a, const MatchOutcome& b) const {
    // 1. 최종 승리 가능한 경우가 있는 카드 우선
    bool aCanWin = (a.finalWinCount > 0);
    bool bCanWin = (b.finalWinCount > 0);
    if (aCanWin != bCanWin) {
        return aCanWin;
    }

    // 2. 승리 경우의 수 많은 카드
    if (a.finalWinCount != b.finalWinCount) {
        return a.finalWinCount > b.finalWinCount;
    }

    // 3. 승리가 안 되면 비김 경우의 수 많은 카드
    if (a.finalDrawCount != b.finalDrawCount) {
        return a.finalDrawCount > b.finalDrawCount;
    }

    // 4. 그래도 같으면 패배 경우의 수 적은 카드
    if (a.finalLoseCount != b.finalLoseCount) {
        return a.finalLoseCount < b.finalLoseCount;
    }

    // 5. 마지막으로 가중 점수
    if (a.weightedScore != b.weightedScore) {
        return a.weightedScore > b.weightedScore;
    }

    return false;
}

ClimbingGame::MatchOutcome ClimbingGame::solveWorstCaseHard(
    const std::vector<Role>& playerState,
    const std::vector<Role>& computerState,
    int currentPlayerScore,
    int currentComputerScore,
    int depth
) const {
    if (playerState.empty() || computerState.empty()) {
        if (currentComputerScore > currentPlayerScore) {
            return MatchOutcome(1, 0, 0, 100.0);
        }
        else if (currentComputerScore == currentPlayerScore) {
            return MatchOutcome(0, 1, 0, 0.0);
        }
        else {
            return MatchOutcome(0, 0, 1, -100.0);
        }
    }

    MatchOutcome bestOutcome;
    bool hasBest = false;

    for (int i = 0; i < (int)computerState.size(); i++) {
        MatchOutcome candidate = evaluateComputerChoiceHard(
            i,
            playerState,
            computerState,
            currentPlayerScore,
            currentComputerScore,
            depth
        );

        if (!hasBest || isBetterHardOutcome(candidate, bestOutcome)) {
            bestOutcome = candidate;
            hasBest = true;
        }
    }

    return bestOutcome;
}

ClimbingGame::MatchOutcome ClimbingGame::evaluateComputerChoiceHard(
    int computerIndex,
    const std::vector<Role>& playerState,
    const std::vector<Role>& computerState,
    int currentPlayerScore,
    int currentComputerScore,
    int depth
) const {
    Role computerCard = computerState[computerIndex];

    MatchOutcome worstForComputer;
    bool initialized = false;

    // 플레이어는 컴퓨터에게 가장 불리한 수를 낸다고 가정
    for (int p = 0; p < (int)playerState.size(); p++) {
        Role playerCard = playerState[p];

        int nextPlayerScore = currentPlayerScore;
        int nextComputerScore = currentComputerScore;

        int result = judgeRound(playerCard, computerCard);

        double immediateScore = 0.0;
        if (result == 1) {
            nextPlayerScore++;
            immediateScore = -1.0;
        }
        else if (result == -1) {
            nextComputerScore++;
            immediateScore = 1.0;
        }

        std::vector<Role> nextPlayerState = playerState;
        std::vector<Role> nextComputerState = computerState;

        nextPlayerState.erase(nextPlayerState.begin() + p);
        nextComputerState.erase(nextComputerState.begin() + computerIndex);

        MatchOutcome future = solveWorstCaseHard(
            nextPlayerState,
            nextComputerState,
            nextPlayerScore,
            nextComputerScore,
            depth + 1
        );

        double weight = getPatternWeight(playerCard, depth);

        MatchOutcome current(
            future.finalWinCount,
            future.finalDrawCount,
            future.finalLoseCount,
            future.weightedScore + immediateScore * weight
        );

        // 플레이어는 컴퓨터에게 가장 나쁜 분기를 선택
        if (!initialized) {
            worstForComputer = current;
            initialized = true;
        }
        else {
            // current가 컴퓨터 입장에서 더 나쁘면 교체
            if (isBetterHardOutcome(worstForComputer, current)) {
                worstForComputer = current;
            }
        }
    }

    return worstForComputer;
}

int ClimbingGame::getHardChoice() const {
    if (computerCards.empty()) return -1;

    std::vector<MatchOutcome> outcomes(computerCards.size());

    for (int i = 0; i < (int)computerCards.size(); i++) {
        outcomes[i] = evaluateComputerChoiceHard(
            i,
            playerCards,
            computerCards,
            playerScore,
            computerScore,
            roundNumber - 1
        );
    }

    int bestIndex = 0;
    for (int i = 1; i < (int)computerCards.size(); i++) {
        if (isBetterHardOutcome(outcomes[i], outcomes[bestIndex])) {
            bestIndex = i;
        }
    }

    return bestIndex;
}

int ClimbingGame::getComputerChoice() const {
    if (computerCards.empty()) return -1;

    // EASY
    if (difficulty == State::Difficulty::EASY) {
        return getRandomChoice(computerCards);
    }

    // NORMAL
    if (difficulty == State::Difficulty::NORMAL) {
        if (roundNumber == 1) {
            return getRandomChoice(computerCards);
        }

        int roll = std::rand() % 100;
        if (roll < 30) {
            return getRandomChoice(computerCards);
        }

        return getBestImmediateChoice();
    }

    // HARD
    return getHardChoice();
}

int ClimbingGame::judgeRound(Role playerCard, Role computerCard) const {
    if (playerCard == computerCard) return 0;

    if (playerCard == Role::KING) {
        if (computerCard == Role::QUEEN || computerCard == Role::SERVANT) return 1;
        return -1;
    }

    if (playerCard == Role::QUEEN) {
        if (computerCard == Role::SERVANT || computerCard == Role::JOKER) return 1;
        return -1;
    }

    if (playerCard == Role::SERVANT) {
        if (computerCard == Role::JOKER) return 1;
        return -1;
    }

    if (computerCard == Role::KING) return 1;
    return -1;
}

void ClimbingGame::removeUsedCard(std::vector<Role>& cards, int index) {
    if (index >= 0 && index < (int)cards.size()) {
        cards.erase(cards.begin() + index);
    }
}

void ClimbingGame::recordPlayerCard(Role role) {
    currentSetPlayerHistory.push_back(role);
}

void ClimbingGame::saveCurrentSetHistory() {
    for (int i = 0; i < (int)currentSetPlayerHistory.size() && i < 4; i++) {
        playerHistoryByRound[i].push_back(currentSetPlayerHistory[i]);
    }
}

void ClimbingGame::finishCurrentSet() {
    if (playerScore > computerScore) {
        playerSetWins++;
    }
    else if (playerScore < computerScore) {
        computerSetWins++;
    }

    saveCurrentSetHistory();
}

void ClimbingGame::play() {
    while (true) {
        setupMatch();
        bool restartMatch = false;

        while (currentSet <= maxSets) {
            setupGame();
            bool restartGame = false;

            while (roundNumber <= maxRounds && !playerCards.empty() && !computerCards.empty()) {
                int playerIndex = selectPlayerCard();

                if (playerIndex == -2) {
                    restartGame = true;
                    break;
                }
                if (playerIndex == -3) {
                    return;
                }

                int computerIndex = getComputerChoice();

                Role playerCard = playerCards[playerIndex];
                Role computerCard = computerCards[computerIndex];

                recordPlayerCard(playerCard);

                int result = judgeRound(playerCard, computerCard);

                if (result == 1) {
                    playerScore++;
                }
                else if (result == -1) {
                    computerScore++;
                }

                removeUsedCard(playerCards, playerIndex);
                removeUsedCard(computerCards, computerIndex);

                std::string playerName = roleToString(playerCard);
                std::string computerName = roleToString(computerCard);

                view.drawRoundResult(playerName,
                    computerName,
                    view.getRoleArt(playerName),
                    view.getRoleArt(computerName),
                    result,
                    playerScore,
                    computerScore,
                    currentSet,
                    maxSets,
                    playerSetWins,
                    computerSetWins);

                roundNumber++;
            }

            if (restartGame) {
                restartMatch = true;
                break;
            }

            finishCurrentSet();

            // 기존 최종 결과 화면 재사용
            view.drawFinalResult(playerScore, computerScore);

            currentSet++;
        }

        if (restartMatch) {
            continue;
        }

        // 3판 매치 종료 후 최종 승자 결정
        view.drawFinalResult(playerSetWins, computerSetWins);
        return;
    }
}