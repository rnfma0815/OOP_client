#pragma once
#include <vector>
#include "Card.h"
#include "State.h"

class BlackjackDealerLogic {
public:
    bool shouldOpen(
        State::Difficulty difficulty,
        int dealerScore,
        int playerScore,
        const std::vector<Card>& remainingCards
    ) const;

private:
    bool shouldOpenEasy(
        int dealerScore
    ) const;

    bool shouldOpenNormal(
        int dealerScore,
        int playerScore
    ) const;

    bool shouldOpenHard(
        int dealerScore,
        int playerScore,
        const std::vector<Card>& remainingCards
    ) const;

    int calculateSafeDrawCount(
        int dealerScore,
        const std::vector<Card>& remainingCards
    ) const;

    int calculateCatchUpCount(
        int dealerScore,
        int playerScore,
        const std::vector<Card>& remainingCards
    ) const;

    int getCardValueForDealer(const Card& card, int currentScore) const;
};