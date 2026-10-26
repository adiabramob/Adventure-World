#include "GameRecorder.h"

void GameRecorder::handleRiddle(Player& player) {
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
            cls();
            std::cout << "Riddle: " << r.getQuestion() << std::endl;
            std::cout << "\nYour answer: ";

            std::string answer;
            std::cin >> answer;

            bool isCorrect = (answer == r.getAnswer());
            std::string status = isCorrect ? "CORRECT" : "WRONG";

            std::string logMsg = "RIDDLE_EVENT | Q: " + r.getQuestion() +
                " | User_Answer: " + answer +
                " | Result: " + status;
            logEvent(logMsg);

            if (isCorrect) {
                std::cout << "Correct!" << std::endl;
                r.solve();
                player.addScore(10);
                Sleep(700);
                player.forceMoveTo(r.getX(), r.getY());
            }
            else {
                std::cout << "Wrong! You stay here... \n";
                player.subtractScore(10);
                Sleep(700);
                if (player.getScore() <= 0) isRunning = false;
            }

            cls();
            screen.draw();
            player1.draw();
            player2.draw();
            return;
        }
    }
}

void GameRecorder::handleHint(Player& player) {
    int nx = player.getX();
    int ny = player.getY();

    switch (player.getCurrentDirection()) {
    case Direction::UP:    ny -= 1; break;
    case Direction::DOWN:  ny += 1; break;
    case Direction::LEFT:  nx -= 1; break;
    case Direction::RIGHT: nx += 1; break;
    default: return;
    }

    for (Hint& h : hints) {
        if (!h.destroyed() && h.getX() == nx && h.getY() == ny && h.getRoomID() == screen.getCurrentScreenID()) {

            player.setDirection(Direction::STAY);

            Game::handleHint(player);

            logEvent("HINT_OPENED | Message: " + h.getMessage());
            return;
        }
    }
}
