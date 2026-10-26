#include "Bonus.h"
#include "Game.h"

//from chatgpt
void Bonus::setColorByType(char symbol) {
    if (!Game::useColors) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (symbol) {
    case '$': SetConsoleTextAttribute(hConsole, 12); break;
    case '&': SetConsoleTextAttribute(hConsole, 13); break;
    case '#': SetConsoleTextAttribute(hConsole, 14); break;
    case 'K': SetConsoleTextAttribute(hConsole, 9); break;
    case 'S': SetConsoleTextAttribute(hConsole, 13); break;
    case '?': SetConsoleTextAttribute(hConsole, 5); break;
    case '*': SetConsoleTextAttribute(hConsole, 1); break;
    case 'w': SetConsoleTextAttribute(hConsole, 8); break;
    case '!': SetConsoleTextAttribute(hConsole, 11); break;
    case '@': SetConsoleTextAttribute(hConsole, 6); break;
    default: SetConsoleTextAttribute(hConsole, 15); break;
    }
}
