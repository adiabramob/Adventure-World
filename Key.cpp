#include "Key.h"
#include "Bonus.h"

void Key::draw() const {
    if (!pickedUp) {
        gotoxy(position.getX(), position.getY());

        char keySymbol = static_cast<char>(Symbol::KEY_CHAR);

        Bonus::setColorByType(keySymbol);
        std::cout << keySymbol;
        Bonus::resetColor();
    }
}
