#pragma once
#include "State.h"

class Game {
protected:
    State::Difficulty difficulty = State::Difficulty::EASY;

public:
    virtual void play() = 0;
    virtual ~Game() {}

    void setDifficulty(State::Difficulty d) {
        difficulty = d;
    }

protected:
    State::PauseResult showPauseMenu();
};