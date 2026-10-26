#pragma once
#include "Point.h"

class Switch {
private:
	int x, y;
	bool isOn = false;
	int roomID;
	int switchID;
	bool destroyFlag = false;
public:
	Switch(int x, int y, int roomID, int id)
		:x(x), y(y), roomID(roomID), switchID(id) {}
	int getID() const { return switchID; }
	bool getState() const { return isOn; }
	void toggle() { isOn = !isOn; }
	int getX() const { return x; }
	int getY() const { return y; }
	int getRoomID() const { return roomID; }
	void draw() const;
	void destroy() {
		destroyFlag = true;
		gotoxy(x, y);
		std::cout << ' ';
	}
	bool destroyed() const { return destroyFlag; }
};
