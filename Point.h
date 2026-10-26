#pragma once
#include <iostream>
#include "Direction.h"
#include "Utils.h"

class Point {
private:
	int x = 1;
	int y = 1;
	int diff_x = 0;
	int diff_y = 0;
	char symbol = ' ';

public:
	Point() = default;
	Point(int x, int y, int dx, int dy, char symbol) : x(x), y(y), diff_x(dx), diff_y(dy), symbol(symbol) {}

	int getX() const { return x; }
	int getY() const { return y; }
	char getSymbol() const { return symbol; }

	void setX(int newX) { x = newX; }
	void setY(int newY) { y = newY; }

	void draw(char c) {
		gotoxy(x, y);
		std::cout << c;
	}
	void move();
	void setDirection(Direction dir);
};
