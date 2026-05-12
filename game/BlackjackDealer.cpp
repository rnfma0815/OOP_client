#include "BlackjackDealer.h"
#include <cstdlib>

BlackjackDealer::BlackjackDealer() : Dealer() {}

int BlackjackDealer::calculateScore() const {
    int total = 0;
    int aceCount = 0;

    for (const Card& card : openedCards) {
        total += card.getValue();
        if (card.getRank() == "A") {
            aceCount++;
        }
    }

    while (total > 21 && aceCount > 0) {
        total -= 10;
        aceCount--;
    }

    return total;
}

int BlackjackDealer::getNextScoreWithCard(const Card& card) const {
    int total = 0;
    int aceCount = 0;

    for (const Card& opened : openedCards) {
        total += opened.getValue();
        if (opened.getRank() == "A") {
            aceCount++;
        }
    }

    total += card.getValue();
    if (card.getRank() == "A") {
        aceCount++;
    }

    while (total > 21 && aceCount > 0) {
        total -= 10;
        aceCount--;
    }

    return total;
}

bool BlackjackDealer::shouldOpenNormal(int playerScore) const {
    int dealerScore = calculateScore();

    // 플레이어가 버스트면 굳이 더 열 필요 없음
    if (playerScore > 21) {
        return false;
    }

    // 14 이하면 무조건 오픈
    if (dealerScore <= 14) {
        return true;
    }

    // 이미 이기고 있으면 보통 난이도는 좀 더 단순하게 멈춤
    if (dealerScore > playerScore) {
        if (dealerScore >= 17) {
            return false;
        }
        else {
            // 15~16에서 조금 욕심
            return (std::rand() % 100 < 30);
        }
    }

    // 지고 있거나 비기고 있으면 17 전까지는 도전
    if (dealerScore < 17) {
        return true;
    }

    return false;
}

bool BlackjackDealer::shouldOpenHard(int playerScore) const {
    int dealerScore = calculateScore();

    if (playerScore > 21) {
        return false;
    }

    if (dealerScore <= 14) {
        return true;
    }

    // 이미 이기고 있는 상황
    if (dealerScore > playerScore) {
        if (dealerScore >= 17) {
            return false;
        }
        else {
            // 15~16이면 살짝 욕심
            return (std::rand() % 100 < 30);
        }
    }

    // 남은 카드가 없으면 멈춤
    if (roundCards.empty()) {
        return false;
    }

    int currentDiff = 21 - dealerScore;

    int burstCount = 0;
    int improveCount = 0;
    int beatPlayerCount = 0;

    for (const Card& card : roundCards) {
        int nextScore = getNextScoreWithCard(card);

        if (nextScore > 21) {
            burstCount++;
            continue;
        }

        int nextDiff = 21 - nextScore;

        // 21에 더 가까워지거나 정확히 21이 되는 경우
        if (nextDiff < currentDiff) {
            improveCount++;
        }

        // 마지막 도박수용
        if (nextScore > playerScore && nextScore <= 21) {
            beatPlayerCount++;
        }
    }

    int total = (int)roundCards.size();
    double burstRate = (double)burstCount / total;
    double improveRate = (double)improveCount / total;

    // 플레이어보다 불리한 상태면 기준 완화
    bool behindPlayer = (dealerScore <= playerScore);

    double improveOpenThreshold = behindPlayer ? 0.40 : 0.50;
    double burstAllowThreshold = behindPlayer ? 0.60 : 0.50;

    // 좋아질 확률이 높고 버스트 위험이 감당 가능하면 오픈
    if (improveRate >= improveOpenThreshold && burstRate <= burstAllowThreshold) {
        return true;
    }

    // 애매한 구간이면 반반
    if (improveRate >= 0.30 && improveRate < 0.40) {
        return (std::rand() % 100 < 50);
    }

    // 마지막 도박수
    double beatPlayerRate = (double)beatPlayerCount / total;
    if (beatPlayerRate >= 0.30) {
        return true;
    }

    return false;
}

void BlackjackDealer::playTurn(int playerScore, State::Difficulty difficulty) {
    // 첫 장은 무조건 오픈
    if (openedCards.empty() && hasRoundCard()) {
        openNextCard();
    }

    while (hasRoundCard()) {
        bool shouldOpen = false;

        if (difficulty == State::Difficulty::EASY) {
            shouldOpen = (calculateScore() < 17);
        }
        else if (difficulty == State::Difficulty::NORMAL) {
            shouldOpen = shouldOpenNormal(playerScore);
        }
        else {
            shouldOpen = shouldOpenHard(playerScore);
        }

        if (!shouldOpen) {
            break;
        }

        openNextCard();

        if (calculateScore() >= 21) {
            break;
        }
    }
}