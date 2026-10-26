#pragma once
#include "Point.h"
#include "Switch.h"
#include "Player.h"
#include <vector>

class Door {
public:
	enum class DoorID {
		ONE = '1',
		TWO = '2',
		THREE = '3',
		FOUR = '4',
		FIVE = '5',
		SIX = '6',
		SEVEN = '7',
		EIGHT = '8',
		NINE = '9'
	};
private:
	Point position;
	DoorID id;
	int roomID;
	bool isOpen;
	int targetRoomID;
	std::vector<int> linkedSwitches;
	bool isDestroyed = false;
public:
	Door(int x, int y, DoorID id, int room, int targetRoom)
		: position(x, y, 0, 0, ' '), id(id), isOpen(false), roomID(room), targetRoomID(targetRoom) {}

	void linkSwitch(int switchID) {
		linkedSwitches.push_back(switchID);
	}
	bool canOpen(const std::vector<Switch>& switches) const;
	int getID() const { return static_cast<char>(id); }
	int getX() const { return position.getX(); }
	int getY() const { return position.getY(); }
	bool getIsOpen() const { return isOpen; }
	int getRoomID() const { return roomID; }
	int getTarget() const { return targetRoomID; }
	void setTarget(int id) { targetRoomID = id; }
	void open() { isOpen = true; }
	bool hasLinkedSwitches() const {
		return !linkedSwitches.empty();
	}
	bool isOpenNow(const std::vector<Player>& players, const std::vector<Switch>& switches) const;
	void destroy() {
		isDestroyed = true;
		isOpen = false;
		gotoxy(getX(), getY());
		std::cout << ' ';
	}

	bool destroyed() const { return isDestroyed; }
};
