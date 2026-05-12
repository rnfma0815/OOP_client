#include "Game.h"
#include "PauseMenuView.h"

State::PauseResult Game::showPauseMenu() {
    PauseMenuView pauseMenuView;
    return pauseMenuView.run();
}
