#pragma once
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Screen.h"
#include "Player.h"
#include "Door.h"
#include "Key.h"
#include "Riddle.h"
#include "Obstacle.h"
#include "Switch.h"
#include "Spring.h"
#include "HUD.h"
#include "Torch.h"
#include "Bomb.h"
#include "Hint.h"
#include <vector>

enum class Keys : int {
	UP_P1 = 'w',
	DOWN_P1 = 'x',
	LEFT_P1 = 'a',
	RIGHT_P1 = 'd',
	STAY_P1 = 's',

	UP_P2 = 'i',
	DOWN_P2 = 'm',
	LEFT_P2 = 'j',
	RIGHT_P2 = 'l',
	STAY_P2 = 'k',

	ESC = 27,
	BACK_TO_MENU_L = 'h',
	BACK_TO_MENU_U = 'H'
};

class Game {
protected:
	Screen screen;
	Player player1;
	Player player2;
	std::vector<Door> doors;
	std::vector<Key> keys;
	std::vector<Obstacle> obstacles;
	std::vector<Switch> switches;
	std::vector<Riddle> riddles;
	std::vector<Hint> hints;
	std::vector<Spring> springs;
	std::vector<Torch> torches;
	std::vector<Bomb> bombs;

	bool isRunning = true;
	int gameCycle = 0;

public:
	Game();
	virtual ~Game() = default;
	virtual void start();
	virtual void logEvent(const std::string& msg) {};
	static bool useColors;
	virtual void handleRiddle(Player& player);
	virtual void handleHint(Player& player);

protected:
	virtual void handleInput();
	virtual void update();
	virtual void draw();
	void pauseGame();
	void checkRoomTransition();
	bool isOutOfBounds(const Point& point) const;
	void openDoorOnScreen(Door& door) {
		screen.setScreenChar(door.getX(), door.getY(), ' ');
	}
	Door* getDoorAt(int x, int y);
	void drawDarkRoom();
	void drawDarkRoomWithTorch();
	void executeCommand(int ch);
	void checkPlayersLives();
	void transferSpring(Player& from, Player& to);
	void handlePlayerCollisions(Player& p, Player& other);
	bool loadRiddlesFromFile(const std::string& filename);
	void loadHints(const std::string& filename);
};
