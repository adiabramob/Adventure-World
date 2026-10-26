#include "Utils.h"
#include <iostream>
#include <windows.h>
#include <cstdlib>

//from GitHub Copilot
void gotoxy(int x, int y) {
	std::cout.flush();
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//from Chat GPT
void hideCursor() {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(hStdOut, &curInfo);
	curInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hStdOut, &curInfo);
}

//from Chat GPT
void cls() {
	system("cls");
}
