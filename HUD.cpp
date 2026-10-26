#include "HUD.h"

//from chatgpt
void HUD::draw(const Player& player1, const Player& player2, int legendX, int legendY) {
    int x = legendX;
    int y = legendY;

    gotoxy(x, y);
    std::cout << "P1: ";
    for (int i = 0; i < player1.getLives(); ++i)
        std::cout << "L ";
    std::cout << "Item: ";
    std::cout << (player1.getHaldItem() != ' ' ? player1.getHaldItem() : '-');

    gotoxy(x, y + 1);
    std::cout << "P2: ";
    for (int i = 0; i < player2.getLives(); ++i)
        std::cout << "L ";
    std::cout << "Item: ";
    std::cout << (player2.getHaldItem() != ' ' ? player2.getHaldItem() : '-');

    gotoxy(x, y + 2);
    std::cout << "Total Score: "
        << player1.getScore() + player2.getScore();
}
