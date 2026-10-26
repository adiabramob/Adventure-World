#pragma once
#include "Point.h"
#include "Direction.h"
#include "Utils.h"
#include <vector>
#include "Bonus.h"
#include "Switch.h"
#include "Key.h"
#include "Riddle.h"

class Screen;   // Forward declaration
class Obstacle; // Forward declaration
class Door;     // Forward declaration
class Game;     // Forward declaration
class Hint;     // Forward declaration

class Player {
private:
    Point position;
    Direction currentDirection = Direction::STAY;
    bool isMoving = false;
    bool stayMode = true;
    int speed = 1;
    int lastX = 0;
    int lastY = 0;
    char haldItem = ' ';
    int springBoostSteps = 0;
    int launch_turns = 0;
    int launch_dx = 0;
    int launch_dy = 0;
    int springBoostTime = 0;
    Direction springBoostDir = Direction::STAY;
    Direction lateralDir = Direction::STAY;
    int lives = 3;
    int score = 0;

public:
    Player() = default;
    Player(int x, int y, char symbol) : position(x, y, 0, 0, symbol), lives(3), score(0) {
        lastX = x;
        lastY = y;
    }

    int getX() const { return position.getX(); }
    int getY() const { return position.getY(); }
    void setX(int newX) { position.setX(newX); }
    void setY(int newY) { position.setY(newY); }
    void setPosition(int x, int y) {
        position.setX(x);
        position.setY(y);
    }
    void setSpringBoost(int steps, Direction dir) {
        springBoostSteps = steps;
        springBoostDir = dir;
    }

    int getSpringBoostSteps() const { return springBoostSteps; }
    Direction getSpringBoostDir() const { return springBoostDir; }

    char getSymbol() const { return position.getSymbol(); }
    int getSpeed() const { return speed; }
    Direction getCurrentDirection() const { return currentDirection; }
    char getHaldItem() const { return haldItem; }
    int getLives() const { return lives; }
    int getScore() const { return score; }
    int getForce() const { return speed; }
    void loseLife() { if (lives > 0) lives--; }
    void addScore(int pts) { score += pts; }
    void subtractScore(int pts) {
        score -= pts;
        if (score < 0) score = 0;
    }

    void draw();
    void erase();

    void move(const Screen& screen);
    void stopMovement();
    void forceMoveTo(int x, int y);

    void setDirection(Direction dir);
    void setSpeed(int s) { speed = s; }

    void pickHaldItem(char item) { haldItem = item; }
    void dropHaldItem() { haldItem = ' '; }
    bool canMove(const Screen& screen, const std::vector<Door>& doors, const std::vector<Player>& players, const std::vector<Obstacle>& obstacles, const std::vector<Switch>& switches) const;

    void setSpringBoost(int steps) {
        springBoostSteps = steps;
        springBoostDir = currentDirection;
    }
    void startSpringLaunch(int steps, Direction dir) {
        springBoostSteps = steps;
        springBoostDir = dir;
        launch_turns = steps * steps;
    }

    bool isSpringLaunching() const {
        return springBoostSteps > 0;
    }
    void moveWithSpeed(
        const Screen& screen,
        const std::vector<Door>& doors,
        const std::vector<Player>& players,
        std::vector<Obstacle>& obstacles,
        const std::vector<Switch>& switches,
        std::vector<Key>& keys,
        std::vector<Riddle>& riddles,
        std::vector<Hint>& hints,
        Game& game
    );

    void setSpringBoostTime(int t) { springBoostTime = t; }
    int getSpringBoostTime() const { return springBoostTime; }
    void decreaseSpringBoostTime() { if (springBoostTime > 0) springBoostTime--; }
    void setSpringBoostDir(Direction d) {
        springBoostDir = d;
    }
};
