#include "GameReplay.h"
#include <iostream>
#include <sstream> 

GameReplay::GameReplay(bool isSilent) : Game(), silent(isSilent) {
    loadSteps();
    loadExpectedResults();
}

void GameReplay::loadSteps() {
    std::ifstream stepsFile("adv-world.steps");
    if (!stepsFile.is_open()) return;

    std::string screensHeader;
    std::getline(stepsFile, screensHeader);

    long cycle;
    char key;
    while (stepsFile >> cycle >> key) {
        recordedSteps[cycle] = key;
    }
    stepsFile.close();
}

void GameReplay::loadExpectedResults() {
    std::ifstream resFile("adv-world.result");
    if (!resFile.is_open()) return;

    std::string line;
    while (std::getline(resFile, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        long cycle;
        if (ss >> cycle) {
            std::string event;
            std::getline(ss, event);

            if (!event.empty()) {
                size_t first = event.find_first_not_of(" \t");
                if (first != std::string::npos) {
                    event = event.substr(first);
                }
            }

            expectedResults[cycle] = event;
        }
    }
    resFile.close();
}

void GameReplay::handleInput() {
    if (recordedSteps.count(gameCycle)) {
        executeCommand(recordedSteps[gameCycle]);
    }
}

void GameReplay::logEvent(const std::string& msg) {
    if (expectedResults.count(gameCycle)) {
        std::string expected = expectedResults[gameCycle];

        if (expected != msg) {
            testPassed = false;
            mismatchReason = "Mismatch at Cycle " + std::to_string(gameCycle) +
                "\nExpected: " + expected +
                "\nGot:      " + msg;
        }
    }
}

void GameReplay::start() {
    hideCursor();
    if (!silent) screen.draw();

    long lastStep = recordedSteps.empty() ? 0 : recordedSteps.rbegin()->first;
    long lastRes = expectedResults.empty() ? 0 : expectedResults.rbegin()->first;
    long finalCycle = (lastStep > lastRes) ? lastStep : lastRes;

    while (isRunning && gameCycle <= finalCycle) {
        handleInput();
        update();

        if (!silent) {
            draw();
            checkRoomTransition();
            Sleep(30);
        }
        else {
            checkRoomTransition();
        }
        gameCycle++;
    }

    cls();
    if (testPassed) {
        std::cout << "test passed" << std::endl;
    }
    else {
        std::cout << "test failed" << std::endl;
        std::cout << mismatchReason << std::endl;
    }

    if (silent) Sleep(2000);
}

GameReplay::~GameReplay() {}

void GameReplay::handleRiddle(Player& player) {
    int nx = player.getX();
    int ny = player.getY();

    switch (player.getCurrentDirection()) {
    case Direction::UP:    ny -= 1; break;
    case Direction::DOWN:  ny += 1; break;
    case Direction::LEFT:  nx -= 1; break;
    case Direction::RIGHT: nx += 1; break;
    default: return;
    }

    for (Riddle& r : riddles) {
        if (!r.isSolved() && r.getX() == nx && r.getY() == ny && r.getRoomID() == screen.getCurrentScreenID()) {
            player.setDirection(Direction::STAY);

            std::string recordedInFile = "";
            std::string answerFromLog = "unknown";

            if (expectedResults.count(gameCycle)) {
                recordedInFile = expectedResults[gameCycle];

                size_t ansPos = recordedInFile.find("User_Answer: ");
                if (ansPos != std::string::npos) {
                    ansPos += 13;
                    size_t endPos = recordedInFile.find(" |", ansPos);
                    if (endPos != std::string::npos) {
                        answerFromLog = recordedInFile.substr(ansPos, endPos - ansPos);
                    }
                }
            }

            bool isActuallyCorrect = (answerFromLog == r.getAnswer());
            std::string currentStatus = isActuallyCorrect ? "CORRECT" : "WRONG";

            std::string currentReplayEvent = "RIDDLE_EVENT | Q: " + r.getQuestion() +
                " | User_Answer: " + answerFromLog +
                " | Result: " + currentStatus;

            logEvent(currentReplayEvent);

            if (isActuallyCorrect) {
                r.solve();
                player.addScore(10);
                player.forceMoveTo(r.getX(), r.getY());
            }
            else {
                player.subtractScore(10);
                if (player.getScore() <= 0) isRunning = false;
            }

            if (!silent) {
                cls();
                std::cout << "[REPLAY] Riddle Encountered: " << r.getQuestion() << std::endl;
                std::cout << "Original Answer: " << answerFromLog << " (" << currentStatus << ")" << std::endl;
                Sleep(1000);
                cls();
                screen.draw();
                player1.draw();
                player2.draw();
            }
            return;
        }
    }
}

void GameReplay::handleHint(Player& player) {}