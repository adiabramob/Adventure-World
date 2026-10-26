#pragma once
#include "Point.h"
#include <string>
#include <vector>

class Riddle {
public:
	enum class Symbol : char { RIDDLE_CHAR = '?' };
private:
	Point position;
	std::string question;
	std::string answer;
	bool solved;
	int roomID;
	bool destroyFlag = false;
public:
	Riddle(int x, int y, const std::string& q, const std::string& a, int room)
		: position(x, y, 0, 0, static_cast<char>(Symbol::RIDDLE_CHAR)), question(q), answer(a), solved(false), roomID(room) {
	}

	int getX() const { return position.getX(); }
	int getY() const { return position.getY(); }
	char getSymbol() const { return position.getSymbol(); }
	int getRoomID() const { return roomID; }

	bool isSolved() const { return solved; }
	void solve() { solved = true; }

	const std::string& getQuestion() const { return question; }
	const std::string& getAnswer() const { return answer; }

	Point getPosition() const { return position; }

	void destroy() { destroyFlag = true; position.draw(' '); }
	bool destroyed() const { return destroyFlag; }
	std::vector<Point> getPoints() const {
		return { position };
	}
	void draw() const;
};
