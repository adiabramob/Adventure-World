#pragma once
#include "Game.h"
#include <map>
#include <fstream>
#include <string>
#include <vector>

class GameReplay : public Game {
private:
    std::map<long, char> recordedSteps;
    std::map<long, std::string> expectedResults;
    bool silent;
    bool testPassed = true;
    std::string mismatchReason = "";

    void loadSteps();
    void loadExpectedResults();

public:
    GameReplay(bool isSilent);
    virtual void handleInput() override;
    virtual void logEvent(const std::string& msg) override;
    virtual void start() override;
    virtual void handleRiddle(Player& player) override;
    virtual void handleHint(Player& player) override;
    ~GameReplay();
};
