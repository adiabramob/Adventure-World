#pragma once
#include "Point.h"
#include "Screen.h"
#include "Player.h"
#include <vector>

class Obstacle {
private:
	std::vector<Point> points;
	int roomID;
	bool destroyFlag = false;
public:
	Obstacle(const std::vector<Point>& pts, int room)
		: points(pts), roomID(room) {}

	void draw() const;
	bool isColliding(int x, int y) const;
	const std::vector<Point>& getPoints() const { return points; }
	int getRoomID() const { return roomID; }
	int getSize() const { return points.size(); }
	void erase() const;
	void releasePlayer(Player& player, const Screen& screen) const;
	int getSize() { return points.size(); }
	bool tryMove(Direction dir, const Player& p1, const Player& p2, const Screen& screen);
	bool isBlocking(int x, int y) const;
	bool canBePushed(Direction dir, const std::vector<Player>& players, const Screen& screen) const;
	void destroy();
	bool destroyed() const { return destroyFlag; }
};
