#pragma once
#include <windows.h>

class Bonus {
public:
    static void setColorByType(char symbol);
    static void resetColor() {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
};
