#include "BlackjackDealerLogic.h"

bool BlackjackDealerLogic::shouldOpen(
    State::Difficulty difficulty,
    int dealerScore,
    int playerScore,
    const std::vector<Card>& remainingCards
) const {
    if (difficulty == State::Difficulty::EASY) {
        return shouldOpenEasy(dealerScore);
    }
    else if (difficulty == State::Difficulty::NORMAL) {
        return shouldOpenNormal(dealerScore, playerScore);
    }
    else {
        return shouldOpenHard(dealerScore, playerScore, remainingCards);
    }
}

bool BlackjackDealerLogic::shouldOpenEasy(int dealerScore) const {
    return dealerScore < 17;
}

bool BlackjackDealerLogic::shouldOpenNormal(int dealerScore, int playerScore) const {
    if (playerScore > 21) {
        return false;
    }

    if (dealerScore < 17) {
        return true;
    }

    if (dealerScore >= playerScore) {
        return false;
    }

    return dealerScore < 21;
}

bool BlackjackDealerLogic::shouldOpenHard(
    int dealerScore,
    int playerScore,
    const std::vector<Card>& remainingCards
) const {
    if (playerScore > 21) {
        return false;
    }

    if (remainingCards.empty()) {
        return false;
    }

    int dealerDiff = 21 - dealerScore;
    int playerDiff = 21 - playerScore;

    int safeCount = 0;       // 안 터지는 카드 수
    int improveCount = 0;    // 현재보다 좋아지는 카드 수
    int catchUpCount = 0;    // 플레이어와 같아지거나 더 유리해지는 카드 수

    for (const Card& card : remainingCards) {
        int nextScore = dealerScore + getCardValueForDealer(card, dealerScore);

        if (nextScore > 21) {
            continue;
        }

        safeCount++;

        int nextDiff = 21 - nextScore;

        if (nextDiff < dealerDiff) {
            improveCount++;
        }

        if (nextDiff <= playerDiff) {
            catchUpCount++;
        }
    }

    double total = static_cast<double>(remainingCards.size());

    double safeProbability = safeCount / total;
    double improveProbability = improveCount / total;
    double catchUpProbability = catchUpCount / total;

    // 이미 플레이어보다 같거나 더 유리하면 멈춤
    if (dealerDiff <= playerDiff) {
        return false;
    }

    // 낮은 점수는 개선 가능성이 크므로 적극적
    if (dealerScore <= 11) {
        return safeProbability >= 0.9;
    }

    // 12 ~ 15 : 안전 + 개선을 본다
    if (dealerScore <= 15) {
        return safeProbability >= 0.70 && improveProbability >= 0.55;
    }

    // 16 : 가장 애매한 구간
    if (dealerScore == 16) {
        if (playerScore >= 19) {
            return safeProbability >= 0.45 && improveProbability >= 0.35;
        }
        if (playerScore >= 17) {
            return safeProbability >= 0.55 && improveProbability >= 0.45;
        }
        return false;
    }

    // 17 이상 : 안전 + 추격 가능성
    return safeProbability >= 0.55 && catchUpProbability >= 0.30;
}

int BlackjackDealerLogic::calculateSafeDrawCount(
    int dealerScore,
    const std::vector<Card>& remainingCards
) const {
    int count = 0;

    for (const Card& card : remainingCards) {
        int nextScore = dealerScore + getCardValueForDealer(card, dealerScore);

        if (nextScore <= 21) {
            count++;
        }
    }

    return count;
}

int BlackjackDealerLogic::calculateCatchUpCount(
    int dealerScore,
    int playerScore,
    const std::vector<Card>& remainingCards
) const {
    int count = 0;

    for (const Card& card : remainingCards) {
        int nextScore = dealerScore + getCardValueForDealer(card, dealerScore);

        if (nextScore <= 21 && nextScore >= playerScore) {
            count++;
        }
    }

    return count;
}

int BlackjackDealerLogic::getCardValueForDealer(const Card& card, int currentScore) const {
    if (card.getRank() == "A") {
        if (currentScore + 11 <= 21) {
            return 11;
        }
        return 1;
    }

    return card.getValue();
}