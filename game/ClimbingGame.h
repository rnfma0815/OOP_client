#pragma once
#include "Game.h"
#include "ClimbingGameView.h"
#include <vector>
#include <string>

class ClimbingGame : public Game {
public:
    enum class Role {
        KING,
        QUEEN,
        SERVANT,
        JOKER
    };

    struct MatchOutcome {
        int finalWinCount;
        int finalDrawCount;
        int finalLoseCount;
        double weightedScore;

        MatchOutcome(int w = 0, int d = 0, int l = 0, double s = 0.0)
            : finalWinCount(w), finalDrawCount(d), finalLoseCount(l), weightedScore(s) {}
    };

private:
    std::vector<Role> playerCards;
    std::vector<Role> computerCards;
    ClimbingGameView view;

    int playerScore;
    int computerScore;
    int roundNumber;
    int maxRounds;

    // 3판 매치용
    int currentSet;          // 1 ~ 3
    int maxSets;             // 3
    int playerSetWins;
    int computerSetWins;

    // 플레이어 패턴 기록
    // history[0] = 1라운드에 낸 카드들
    // history[1] = 2라운드에 낸 카드들
    // history[2] = 3라운드에 낸 카드들
    // history[3] = 4라운드에 낸 카드들
    std::vector<Role> playerHistoryByRound[4];

    // 현재 판에서 플레이어가 실제로 낸 순서
    std::vector<Role> currentSetPlayerHistory;

public:
    ClimbingGame();
    void play() override;

private:
    void setupMatch();
    void setupGame();

    int selectPlayerCard();
    int getComputerChoice() const;

    int judgeRound(Role playerCard, Role computerCard) const;
    std::string roleToString(Role role) const;
    std::string roleToArt(Role role) const;

    void removeUsedCard(std::vector<Role>& cards, int index);
    std::string getDifficultyString() const;

private:
    int getRandomChoice(const std::vector<Role>& cards) const;

    // Normal
    int countImmediateWins(Role computerCard, const std::vector<Role>& playerState) const;
    int countImmediateDraws(Role computerCard, const std::vector<Role>& playerState) const;
    int getBestImmediateChoice() const;

    // Hard
    int getHardChoice() const;

    MatchOutcome evaluateComputerChoiceHard(
        int computerIndex,
        const std::vector<Role>& playerState,
        const std::vector<Role>& computerState,
        int currentPlayerScore,
        int currentComputerScore,
        int depth
    ) const;

    MatchOutcome solveWorstCaseHard(
        const std::vector<Role>& playerState,
        const std::vector<Role>& computerState,
        int currentPlayerScore,
        int currentComputerScore,
        int depth
    ) const;

    bool isBetterHardOutcome(const MatchOutcome& a, const MatchOutcome& b) const;

    // 패턴 가중치
    double getPatternWeight(Role playerCard, int depth) const;
    void recordPlayerCard(Role role);
    void saveCurrentSetHistory();

    // 세트 결과
    void finishCurrentSet();
};