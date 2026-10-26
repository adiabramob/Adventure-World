#include "Riddle.h"
#include "Bonus.h"

void Riddle::draw() const {
    if (!solved && !destroyFlag) {
        gotoxy(position.getX(), position.getY());

        char sym = static_cast<char>(Symbol::RIDDLE_CHAR);

        Bonus::setColorByType(sym);
        std::cout << sym;
        Bonus::resetColor();
    }
}
