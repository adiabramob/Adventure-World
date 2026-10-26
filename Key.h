#pragma once
#include "Point.h"
#include <vector>

class Key {
public:
	enum class Symbol : char { KEY_CHAR = 'K' };
	enum class KeyID {
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
	KeyID id;
	int roomID;
	bool pickedUp = false;
	bool destroyFlag = false;
public:
	Key(int x, int y, KeyID id, int room)
		: position(x, y, 0, 0, 'K'), id(id), roomID(room) {}

	char getIDchar() const { return static_cast<char>(id); }
	KeyID getID() const { return id; }
	int getX() const { return position.getX(); }
	int getY() const { return position.getY(); }
	bool isPickedUp() const { return pickedUp; }
	void pickUp() { pickedUp = true; position.draw(' '); }
	void draw() const;
	int getRoomID() const { return roomID; }
	void destroy() {
		destroyFlag = true;
		pickedUp = true;
		position.draw(' ');
	}

	bool destroyed() const { return destroyFlag; }
};
