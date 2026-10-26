#include "Menu.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>

void Menu::start() {
	bool start = true;
	while (start) {
		clear();
		print();
		char choice = _getch();

		switch (static_cast<MenuOptions>(choice)) {
		case MenuOptions::START_GAME:
			chooseColors();
			open_game();
			break;
		case MenuOptions::INSTRUCTIONS:
			instructions();
			break;
		case MenuOptions::EXIT:
			start = false;
			break;
		default:
			std::cout << "invalid choice, try again";
			break;
		}
	}
}

void Menu::clear() {
	system("cls");
}

void Menu::print() {
	std::cout << "****" << std::endl;
	std::cout << "(1) Start a new game" << std::endl;
	std::cout << "(8) Present instructions and keys" << std::endl;
	std::cout << "(9) EXIT" << std::endl;
}

void Menu::open_game() {
	clear();
	if (recordMode) {
		GameRecorder game;
		game.start();
	}
	else {
		Game game;
		game.start();
	}
}

void Menu::instructions()
{
	clear();
	std::cout << "Instructions:" << std::endl;
	std::cout << "player 1 keays:" << std::endl;
	std::cout << "     W - up" << std::endl;
	std::cout << "     A - left" << std::endl;
	std::cout << "     S - stay" << std::endl;
	std::cout << "     D - right" << std::endl;
	std::cout << "     X - down" << std::endl;
	std::cout << "     E - Dispose item" << std::endl;
	std::cout << "player 2 keys:" << std::endl;
	std::cout << "     I - up" << std::endl;
	std::cout << "     J - left" << std::endl;
	std::cout << "     K - stay" << std::endl;
	std::cout << "     L - right" << std::endl;
	std::cout << "     M - down" << std::endl;
	std::cout << "     O - Dispose item" << std::endl;
	std::cout << "GENERAL:" << std::endl;
	std::cout << "     Press ESC during a game to pause" << std::endl;
	std::cout << "     When pause:" << std::endl;
	std::cout << "          ESC- Continue game" << std::endl;
	std::cout << "          H  - Return to main menu" << std::endl;
	std::cout << "     Press any key to return" << std::endl;
	_getch();
}

void Menu::chooseColors() {
	clear();
	std::cout << "Do you want to play with colors?" << std::endl;
	std::cout << "(1) Yes" << std::endl;
	std::cout << "(2) No" << std::endl;

	char c;
	while (true) {
		c = _getch();
		if (c == '1') {
			Game::useColors = true;
			break;
		}
		else if (c == '2') {
			Game::useColors = false;
			break;
		}
	}
}
