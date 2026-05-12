#pragma once

namespace State {

    enum class Difficulty {
        EASY,
        NORMAL,
        HARD
    };

    enum class PauseResult {
        RESUME,
        RESTART,
        EXIT_TO_MENU
    };

    enum class GameResult {
        CLEAR,
        GAME_OVER
    };

}