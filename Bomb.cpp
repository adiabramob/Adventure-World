#include "Bomb.h"

void Bomb::move(Direction dir, const Screen& screen) {
    int newX = position.getX();
    int newY = position.getY();

    switch (dir) {
    case Direction::UP: newY--; break;
    case Direction::DOWN: newY++; break;
    case Direction::LEFT: newX--; break;
    case Direction::RIGHT: newX++; break;
    default: break;
    }

    if (newX >= 0 && newX < Screen::MAX_X &&
        newY >= 0 && newY < Screen::MAX_Y &&
        screen.getScreenChar(newX, newY) != 'w') {
        position.setX(newX);
        position.setY(newY);
    }
}

void Bomb::updateBomb(Player& p1, Player& p2,
    std::vector<Door>& doors,
    std::vector<Switch>& switches,
    std::vector<Key>& keys,
    std::vector<Spring>& springs,
    std::vector<Riddle>& riddles,
    std::vector<Obstacle>& obstacles,
	std::vector<Hint>& hints,
    Screen& screen)
{
    if (!isArmed || isExploded) return;

    timer--;
    if (timer == 0) {
        explode(p1, p2, doors, switches, keys, springs, riddles, obstacles,hints,screen);
        isExploded = true;
        isArmed = false;
    }
}


void Bomb::explode(Player& p1, Player& p2,
    std::vector<Door>& doors,
    std::vector<Switch>& switches,
    std::vector<Key>& keys,
    std::vector<Spring>& springs,
    std::vector<Riddle>& riddles,
    std::vector<Obstacle>& obstacles,
    std::vector<Hint>& hints,
    Screen& screen)
{
    int px = position.getX();
    int py = position.getY();
    int blastRadius = 3;

    for (int y = py - blastRadius; y <= py + blastRadius; y++) {
        for (int x = px - blastRadius; x <= px + blastRadius; x++) {
            if (x < 0 || x >= Screen::MAX_X || y < 0 || y >= Screen::MAX_Y)
                continue;

            if (std::abs(x - px) > blastRadius || std::abs(y - py) > blastRadius)
                continue;

            if (y <= 4) continue;

            if (p1.getX() == x && p1.getY() == y) p1.loseLife();
            if (p2.getX() == x && p2.getY() == y) p2.loseLife();

            for (auto& d : doors)
                if (d.getX() == x && d.getY() == y) {
                    d.destroy();
                    screen.setScreenChar(x, y, ' ');
                }

            for (auto& s : springs)
                if (s.contains(x, y)) {
                    s.destroy();
                }


            for (auto& sw : switches)
                if (sw.getX() == x && sw.getY() == y) {
                    sw.destroy();
                    screen.setScreenChar(x, y, ' ');
                }

            for (auto& k : keys)
                if (k.getX() == x && k.getY() == y && !k.isPickedUp()) {
                    k.destroy();
                    screen.setScreenChar(x, y, ' ');
                }

            for (auto& r : riddles)
                for (const auto& pt : r.getPoints())
                    if (pt.getX() == x && pt.getY() == y) {
                        r.destroy();
                        screen.setScreenChar(x, y, ' ');
                    }
            for (auto& h : hints) {
                if (h.getX() == x && h.getY() == y && h.getRoomID() == screen.getCurrentScreenID()) {
                    h.destroy();
                    screen.setScreenChar(x, y, ' ');
                }
            }

            for (auto& o : obstacles)
                for (const auto& pt : o.getPoints())
                    if (pt.getX() == x && pt.getY() == y) {
                        o.destroy();
                        screen.setScreenChar(x, y, ' ');
                    }


            char currentChar = screen.getScreenChar(x, y);

            if (currentChar == 'w') {
                if (x > 0 && x < Screen::MAX_X - 1 && y < Screen::MAX_Y - 1) {
                    screen.setScreenChar(x, y, ' ');
                    gotoxy(x, y);
                    std::cout << ' ';
                }
            }
            else {
                screen.setScreenChar(x, y, ' ');
                gotoxy(x, y);
                std::cout << ' ';
            }
        }
    }

    isExploded = true;
}


void Bomb::draw() const {
    if (!isExploded && !isPickedUp) {
        gotoxy(position.getX(), position.getY());
        Bonus::setColorByType('@');
        std::cout << '@';
        Bonus::resetColor();
    }
}

void Bomb::release(Player& player, const Screen& screen) {
    if (!isPickedUp) return;

    int dirs[4][2] = { {0,-1}, {0,1}, {-1,0}, {1,0} };

    for (auto& d : dirs) {
        int nx = player.getX() + d[0];
        int ny = player.getY() + d[1];

        if (screen.getScreenChar(nx, ny) == ' ') {
            position.setX(nx);
            position.setY(ny);
            isPickedUp = false;
            isArmed = true;
            timer = 5;
            return;
        }
    }
}