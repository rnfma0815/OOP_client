#pragma once
#include "Dealer.h"
#include "State.h"
#include <vector>

class BlackjackDealer : public Dealer {
public:
    BlackjackDealer();

    int calculateScore() const override;
    void playTurn(int playerScore, State::Difficulty difficulty) override;

private:
    bool shouldOpenNormal(int playerScore) const;
    bool shouldOpenHard(int playerScore) const;
    int getNextScoreWithCard(const Card& card) const;
};