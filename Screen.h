#pragma once
#include <iostream>
#include "Utils.h"
#include "Player.h"
#include <map>

class Screen {
public:
	static const int MAX_X = 80;
	static const int MAX_Y = 25;

private:
	char screen[4][MAX_Y][MAX_X + 1];
	int currentScreen = 0;
	int legendX[4];
	int legendY[4];
	std::map<int, bool> roomsDarkStatus;
public:
	Screen();
	char getScreenChar(int x, int y) const {
		return screen[currentScreen][y][x];
	}

	void draw() const;

	bool isWall(const Player& player) const {
		return screen[currentScreen][player.getY()][player.getX()] == 'w';
	}

	void drawPlayer(const Player& player) const {
		gotoxy(player.getX(), player.getY());
		std::cout << player.getSymbol();
	}

	void nextScreen(int roomID);

	void setScreenChar(int x, int y, char c);

	int getCurrentScreenID() const { return currentScreen; }

	void loadScreensFromFiles();

	int getLegendX() const { return legendX[currentScreen]; }
	int getLegendY() const { return legendY[currentScreen]; }

	void setDark(int roomId, bool status) {
		roomsDarkStatus[roomId] = status;
	}

	bool isDark(int roomId) const {
		auto it = roomsDarkStatus.find(roomId);
		if (it != roomsDarkStatus.end()) {
			return it->second;
		}
		return false;
	}

	bool isValidPosition(int x, int y) const {
		return (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y);
	}
};
