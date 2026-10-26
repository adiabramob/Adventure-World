#pragma once
#include "Player.h"
#include "Screen.h"
#include <string>

class HUD {
public:
	static void draw(const Player& player1, const Player& player2, int legendX, int legendY);
};
