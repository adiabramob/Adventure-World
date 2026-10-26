#pragma once
#include "Point.h"
#include "Obstacle.h"
#include "Key.h"
#include "Spring.h"
#include "Door.h"
#include "Riddle.h"
#include "Hint.h"
#include <vector>

class Bomb {
private:
	Point position;
	int roomID;
	int timer;
	int radius;
	bool isPickedUp = false;
	bool isArmed = false;
	bool isExploded = false;
public:
	Bomb(int x, int y, int roomID)
		:position(x, y, 0, 0, '@'), roomID(roomID), timer(5), isPickedUp(false), isArmed(false), isExploded(false), radius(3) {}

	int getX() const { return position.getX(); }
	int getY() const { return position.getY(); }
	int getRoomID() const { return roomID; }
	bool exploded() const { return isExploded; }
	bool pickedUp() const { return isPickedUp; }
	void pickUp() {
		isPickedUp = true;
		isArmed = false;
	}
	bool active() const { return isArmed; }
	void move(Direction dir, const Screen& screen);
	void updateBomb(Player& p1, Player& p2,
		std::vector<Door>& doors,
		std::vector<Switch>& switches,
		std::vector<Key>& keys,
		std::vector<Spring>& springs,
		std::vector<Riddle>& riddles,
		std::vector<Obstacle>& obstacles,
		std::vector<Hint>& hints,
		Screen& screen);
	void draw() const;
	void release(Player& player, const Screen& screen);
private:
	void explode(Player& p1, Player& p2,
		std::vector<Door>& doors,
		std::vector<Switch>& switches,
		std::vector<Key>& keys,
		std::vector<Spring>& springs,
		std::vector<Riddle>& riddles,
		std::vector<Obstacle>& obstacles,
		std::vector<Hint>& hints,
		Screen& screen);
};
