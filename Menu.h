#pragma once
#include "Game.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "GameRecorder.h"

enum class MenuOptions {
	START_GAME = '1',
	INSTRUCTIONS = '8',
	EXIT = '9'
};

class Menu {
public:
	Menu(bool record = false) : recordMode(record) {}
	void start();

private:
	bool recordMode;
	void clear();
	void print();
	void open_game();
	void instructions();
	void chooseColors();
};
