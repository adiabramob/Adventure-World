#include "Screen.h"
#include "Player.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

Screen::Screen()
{
    currentScreen = 0;
}

void Screen::draw() const {
    system("cls");
    for (int y = 0; y < MAX_Y; y++) {
        for (int x = 0; x < MAX_X; x++) {
            char c = screen[currentScreen][y][x];
            Bonus::setColorByType(c);
            std::cout << c;
            Bonus::resetColor();
        }
        std::cout << std::endl;
    }
}

void Screen::nextScreen(int roomID) {
    currentScreen = roomID;
    system("cls");
    draw();
}

void Screen::setScreenChar(int x, int y, char c) {
    if (x < 0 || x >= MAX_X) {
        return;
    }
    if (y < 0 || y >= MAX_Y) {
        return;
    }
    screen[currentScreen][y][x] = c;
    gotoxy(x, y);
    std::cout << c;
}

//from gemini
void Screen::loadScreensFromFiles() {
    std::vector<std::string> filenames = {
        "adv-world_01.screen.txt", "adv-world_02.screen.txt", "adv-world_03.screen.txt", "adv-world_04.screen.txt"
    };

    for (size_t i = 0; i < filenames.size(); i++) {
        std::ifstream file(filenames[i]);
        if (!file.is_open()) continue;

        for (int row = 0; row < MAX_Y; row++) {
            for (int col = 0; col < MAX_X; col++) {
                if (row == 0 || row == MAX_Y - 1 || col == 0 || col == MAX_X - 1)
                    screen[i][row][col] = 'w';
                else
                    screen[i][row][col] = ' ';
            }
            screen[i][row][MAX_X] = '\0';
        }

        std::string line;
        int y = 0;
        bool foundLegend = false;

        while (y < MAX_Y && std::getline(file, line)) {
            for (int x = 0; x < MAX_X && x < (int)line.size(); x++) {
                if (line[x] == 'L') {
                    legendX[i] = x;
                    legendY[i] = y;
                    screen[i][y][x] = ' ';
                    foundLegend = true;
                }
                else {
                    screen[i][y][x] = line[x];
                }
            }
            y++;
        }
        file.close();

        for (int x = 0; x < MAX_X; x++) {
            screen[i][0][x] = 'w';
            screen[i][MAX_Y - 1][x] = 'w';
        }
        for (int row = 0; row < MAX_Y; row++) {
            screen[i][row][0] = 'w';
            screen[i][row][MAX_X - 1] = 'w';
        }

        if (!foundLegend) {
            legendX[i] = 1; legendY[i] = 1;
        }
    }
}
