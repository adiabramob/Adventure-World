#include "Player.h"
#include "Screen.h"
#include "Obstacle.h"
#include "Switch.h"
#include "Door.h"
#include "Game.h"
#include "Riddle.h"
#include <iostream>

void Player::draw() {
	gotoxy(position.getX(), position.getY());             
	Bonus::setColorByType(position.getSymbol());           
	std::cout << position.getSymbol();                           
	Bonus::resetColor();                                     
}

void Player::erase() {
    gotoxy(position.getX(), position.getY());
    std::cout << ' ';
}

void Player::move(const Screen& screen)
{
    int dx = 0, dy = 0;

    switch (currentDirection) {
    case Direction::UP:    dy = -1; break;
    case Direction::DOWN:  dy = 1;  break;
    case Direction::LEFT:  dx = -1; break;
    case Direction::RIGHT: dx = 1;  break;
    default:
        return;
    }

    for (int i = 0; i < speed; i++) {
        int nextX = position.getX() + dx;
        int nextY = position.getY() + dy;

        if (screen.getScreenChar(nextX, nextY) != ' ')
            break;

        position.setX(nextX);
        position.setY(nextY);
    }
}

//from chat gpt
void Player::setDirection(Direction dir) {
    if (springBoostTime > 0) {
        if (dir == Direction::STAY)
            return;

        if ((springBoostDir == Direction::UP && dir == Direction::DOWN) ||
            (springBoostDir == Direction::DOWN && dir == Direction::UP) ||
            (springBoostDir == Direction::LEFT && dir == Direction::RIGHT) ||
            (springBoostDir == Direction::RIGHT && dir == Direction::LEFT))
            return;
    }

    currentDirection = dir;
    stayMode = (dir == Direction::STAY);
    isMoving = !stayMode;
    position.setDirection(dir);
}


bool Player::canMove(const Screen& screen, const std::vector<Door>& doors, const std::vector<Player>& players, const std::vector<Obstacle>& obstacles, const std::vector<Switch>& switches) const{
int nextX = position.getX();
	int nextY = position.getY();
	for (int step = 0; step < speed; step++) {
		switch (currentDirection) {
		case Direction::UP:
			nextY -= 1;
			break;
		case Direction::DOWN:
			nextY += 1;
			break;
		case Direction::LEFT:
			nextX -= 1;
			break;
		case Direction::RIGHT:
			nextX += 1;
			break;
		case Direction::STAY:
			return true;
		}
		char nextChar = screen.getScreenChar(nextX, nextY);

		if (nextChar == 'w') {
			return false;
		}

        for (const Door& d : doors) {
            if (d.getRoomID() == screen.getCurrentScreenID()) {

                if (d.getX() == nextX && d.getY() == nextY) {
                    bool hasKey = false;
                    for (const Player& p : players) {
                        if (p.getHaldItem() == static_cast<char>(d.getID())) {
                            hasKey = true;
                            break;
                        }
                    }
                    if (!hasKey && !d.isOpenNow(players, switches)) return false;
                }
            }
        }
        for (const Obstacle& o : obstacles) {
            if (o.getRoomID() == screen.getCurrentScreenID()) {

                if (o.isBlocking(nextX, nextY)) {
                    if (!o.canBePushed(currentDirection, players, screen)) {
                        return false;
                    }
                }
            }
        }
	}
	return true;
}

void Player::stopMovement() {
    if (springBoostTime > 0) return;
    isMoving = false;
    stayMode = true;
    position.setDirection(Direction::STAY);
}

void Player::forceMoveTo(int x, int y) {
	position.setX(x);
	position.setY(y);
}


// with gemini
void Player::moveWithSpeed(
    const Screen& screen,
    const std::vector<Door>& doors,
    const std::vector<Player>& players,
    std::vector<Obstacle>& obstacles,
    const std::vector<Switch>& switches,
    std::vector<Key>& keys,
    std::vector<Riddle>& riddles,
    std::vector<Hint>& hints,
    Game& game
) {
    int dx = 0, dy = 0;
    switch (springBoostDir) {
    case Direction::UP:    dy = -1; break;
    case Direction::DOWN:  dy = 1;  break;
    case Direction::LEFT:  dx = -1; break;
    case Direction::RIGHT: dx = 1;  break;
    default: return;
    }

    int side_dx = 0, side_dy = 0;
    if (springBoostTime > 0) {
        if (springBoostDir == Direction::UP || springBoostDir == Direction::DOWN) {
            if (currentDirection == Direction::LEFT) side_dx = -1;
            else if (currentDirection == Direction::RIGHT) side_dx = 1;
        }
        else {
            if (currentDirection == Direction::UP) side_dy = -1;
            else if (currentDirection == Direction::DOWN) side_dy = 1;
        }
    }

    for (int step = 0; step < speed; step++) {
        int nx = position.getX() + dx + (step == 0 ? side_dx : 0);
        int ny = position.getY() + dy + (step == 0 ? side_dy : 0);

        for (const Player& other : players) {
            if (&other != this && nx == other.getX() && ny == other.getY()) {
                erase();
                position.setX(nx);
                position.setY(ny);
                draw();
                return;
            }
        }

        if (screen.getScreenChar(nx, ny) == 'w')
            return;

        bool doorBlocked = false;
        for (const Door& d : doors) {
            if (d.getRoomID() == screen.getCurrentScreenID() && d.getX() == nx && d.getY() == ny) {
                if (this->getHaldItem() != (char)d.getID() && !d.isOpenNow(players, switches)) {
                    doorBlocked = true; break;
                }
            }
        }
        if (doorBlocked) break;

        bool obstacleBlocked = false;
        for (Obstacle& o : obstacles) {
            if (o.getRoomID() == screen.getCurrentScreenID() && o.isBlocking(nx, ny)) {
                if (!o.tryMove(springBoostDir, players[0], players[1], screen)) {
                    obstacleBlocked = true; break;
                }
            }
        }
        if (obstacleBlocked) break;

        erase();
        position.setX(nx);
        position.setY(ny);
        draw();

        for (Key& k : keys) {
            if (!k.isPickedUp() && k.getRoomID() == screen.getCurrentScreenID()) {
                if (position.getX() == k.getX() && position.getY() == k.getY() && this->haldItem == ' ') {
                    this->pickHaldItem(k.getIDchar());
                    k.pickUp();
                }
            }
        }

        game.handleRiddle(*this);
        if (this->currentDirection == Direction::STAY) return;

        game.handleHint(*this);
        if (this->currentDirection == Direction::STAY) return;
    }
}