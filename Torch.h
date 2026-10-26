#pragma once
#include "Point.h"

class Torch {
private:
	Point position;
	int radius = 10;
	bool isOn;
	bool picked;
	int roomID;

public:
	Torch(int x, int y, int r = 3, int room = 0)
		: position(x, y, 0, 0, '!'), radius(r), isOn(true), picked(false), roomID(room) {
	}

	int getX() const { return position.getX(); }
	int getY() const { return position.getY(); }
	int getRoomID() const { return roomID; }

	bool isPicked() const { return picked; }
	void pickUp() { picked = true; }
	void drop() { picked = false; }

	int getRadius() const { return radius; }
	void turnOn() { isOn = true; }
	void turnOff() { isOn = false; }
	bool isLit() const { return isOn; }

	void draw();
};
