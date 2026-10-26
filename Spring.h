#pragma once
#include "Point.h"
#include "Player.h"
#include "Screen.h"
#include <vector>

class Spring {
private:
	std::vector<Point> points;
	bool active = true;
	char symbol = '#';
	int roomID = 0;
	bool destroyFlag = false;
	bool compressed = false;
public:
	Spring(const std::vector<Point>& pts, int room, bool active, char symbol)
		: points(pts), active(active), symbol(symbol), roomID(room) {
	}

	bool isActive() const { return active; }
	void draw() const;
	void erase();
	int activate(Player& player, const Screen& screen);
	int getRoomID() const { return roomID; }
	void destroy() { destroyFlag = true; erase(); }
	bool destroyed() const { return destroyFlag; }
	bool contains(int x, int y) const;
	void reset();
};
