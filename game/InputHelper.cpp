#include "InputHelper.h"
#include <conio.h>

namespace InputHelper {
    Key readKey() {
        int key = _getch();

        if (key == 27) return Key::ESC;
        if (key == 13) return Key::ENTER;

        if (key == 224 || key == 0) {
            int arrow = _getch();
            if (arrow == 72) return Key::UP;
            if (arrow == 80) return Key::DOWN;
            if (arrow == 75) return Key::LEFT;
            if (arrow == 77) return Key::RIGHT;
            return Key::OTHER;
        }

        return Key::OTHER;
    }
}
