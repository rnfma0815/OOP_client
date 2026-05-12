#pragma once

namespace InputHelper {
    enum class Key {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT,
        ENTER,
        ESC,
        OTHER
    };

    Key readKey();
}
