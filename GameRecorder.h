#pragma once
#include "Game.h"
#include <fstream>

class GameRecorder : public Game {
private:
    std::ofstream stepsFile;
    std::ofstream resultFile;

public:
    GameRecorder() : Game() {
        stepsFile.open("adv-world.steps");
        resultFile.open("adv-world.result");
        stepsFile << "adv-world_01.screen.txt adv-world_02.screen.txt adv-world_03.screen.txt adv-world_04.screen.txt" << std::endl;
    }

    void handleInput() override {
        if (_kbhit()) {
            int ch = _getch();

            stepsFile << gameCycle << " " << (char)ch << std::endl;
            executeCommand(ch);
        }
    }

    void logEvent(const std::string& msg) override {
        resultFile << gameCycle << " " << msg << std::endl;
    }

    ~GameRecorder() {
        stepsFile.close();
        resultFile.close();
    }
    void handleRiddle(Player& player) override;
    void handleHint(Player& player) override;
};
