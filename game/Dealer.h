#pragma once
#include "Player.h"
#include "State.h"

class Dealer : public Player {
public:
    Dealer();
    virtual ~Dealer() {}

    virtual void playTurn(int playerScore, State::Difficulty difficulty) = 0;
};