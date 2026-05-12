#pragma once
#include "State.h"

class PauseMenuView {
public:
    State::PauseResult run() const;
    State::PauseResult runOnline() const;
};
