#include "Spring.h"

void Spring::draw() const {
    if (!active) return;
    for (const Point& p : points) {
        gotoxy(p.getX(), p.getY());
        Bonus::setColorByType('#');
        std::cout << symbol;
        Bonus::resetColor();
    }
}

void Spring::erase() {
    for (const Point& p : points) {
        gotoxy(p.getX(), p.getY());
        std::cout << ' ';
    }
}

int Spring::activate(Player& player, const Screen& screen) {
    if (!active) return 0;

    bool onSpring = false;
    for (const Point& p : points) {
        if (player.getX() == p.getX() && player.getY() == p.getY()) {
            onSpring = true;
            break;
        }
    }

    if (!onSpring) return 0;

    Direction dir = player.getCurrentDirection();
    Direction releaseDir;

    switch (dir) {
    case Direction::UP:    releaseDir = Direction::DOWN; break;
    case Direction::DOWN:  releaseDir = Direction::UP;   break;
    case Direction::LEFT:  releaseDir = Direction::RIGHT; break;
    case Direction::RIGHT: releaseDir = Direction::LEFT; break;
    default: return 0;
    }

    player.setSpringBoostDir(releaseDir);
    player.setDirection(releaseDir);

    active = false;
    erase();

    int boostValue = (int)points.size();

    return boostValue;
}

bool Spring::contains(int x, int y) const {
    for (const auto& p : points) {
        if (p.getX() == x && p.getY() == y)
            return true;
    }
    return false;
}

void Spring::reset() {
    if (active) return;
    active = true;
    draw();
}
