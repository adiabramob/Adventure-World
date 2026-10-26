#include "Obstacle.h"
#include <iostream>

void Obstacle::draw() const {
	for (const Point& p : points) {
		gotoxy(p.getX(), p.getY());
		Bonus::setColorByType('*');
		std::cout << '*';
		Bonus::resetColor();
	}
}

bool Obstacle::isColliding(int x, int y) const {
	for (const Point& p : points) {
		if (p.getX() == x && p.getY() == y) {
			return true;
		}
	}
	return false;
}

//from chatgpt
bool Obstacle::tryMove(Direction dir, const Player& p1, const Player& p2, const Screen& screen) {
	int dx = 0, dy = 0;
	switch (dir) {
	case Direction::UP:    dy = -1; break;
	case Direction::DOWN:  dy = 1;  break;
	case Direction::LEFT:  dx = -1; break;
	case Direction::RIGHT: dx = 1;  break;
	default: return false;
	}

	int totalForce = 0;

	const Player* players[2] = { &p1, &p2 };

	for (const Player* p : players) {
		if (p->getCurrentDirection() != dir)
			continue;

		for (const Point& pt : points) {
			if (p->getX() == pt.getX() - dx &&
				p->getY() == pt.getY() - dy) {

				totalForce += p->getForce();
				break;
			}
		}
	}

	if (totalForce < getSize())
		return false;

	for (const Point& p : points) {
		int newX = p.getX() + dx;
		int newY = p.getY() + dy;
		if (newX < 0 || newX >= Screen::MAX_X ||
			newY < 0 || newY >= Screen::MAX_Y)
			return false;
		if (screen.getScreenChar(newX, newY) != ' ')
			return false;
	}

	erase();
	for (Point& p : points) {
		p.setX(p.getX() + dx);
		p.setY(p.getY() + dy);
	}
	draw();

	return true;
}

void Obstacle::erase() const {
	for (const Point& p : points) {
		gotoxy(p.getX(), p.getY());
		std::cout << ' ';
	}
}

//from chatgpt
void Obstacle::releasePlayer(Player& player, const Screen& screen) const {
	if (!isColliding(player.getX(), player.getY())) {
		return;
	}

	int dirs[4][2] = { {0,-1}, {0,1}, {-1,0}, {1,0} };

	for (const auto& dir : dirs) {
		int newX = player.getX() + dir[0];
		int newY = player.getY() + dir[1];
		if (newX < 0 || newX >= Screen::MAX_X || newY < 0 || newY >= Screen::MAX_Y) {
			continue;
		}

		bool collision = false;
		for (const Point& p : points) {
			if (p.getX() == newX && p.getY() == newY) {
				collision = true;
				break;
			}
		}

		if (!collision && screen.getScreenChar(newX, newY) == ' ') {
			player.forceMoveTo(newX, newY);
			return;
		}
	}
}

bool Obstacle::isBlocking(int x, int y) const {
	for (const Point& p : points) {
		if (p.getX() == x && p.getY() == y) return true;
	}
	return false;
}

bool Obstacle::canBePushed(Direction dir, const std::vector<Player>& players, const Screen& screen) const {
	int dx = 0, dy = 0;
	switch (dir) {
	case Direction::UP:    dy = -1; break;
	case Direction::DOWN:  dy = 1;  break;
	case Direction::LEFT:  dx = -1; break;
	case Direction::RIGHT: dx = 1;  break;
	default: return false;
	}

	int totalForce = 0;

	for (const Player& p : players) {
		if (p.getCurrentDirection() != dir)
			continue;

		for (const Point& pt : points) {
			if (p.getX() == pt.getX() - dx &&
				p.getY() == pt.getY() - dy) {

				totalForce += p.getForce();
				break;
			}
		}
	}

	if (totalForce < getSize())
		return false;

	for (const Point& pt : points) {
		int newX = pt.getX() + dx;
		int newY = pt.getY() + dy;

		if (newX < 0 || newX >= Screen::MAX_X ||
			newY < 0 || newY >= Screen::MAX_Y)
			return false;

		if (screen.getScreenChar(newX, newY) != ' ')
			return false;
	}

	return true;
}

void Obstacle::destroy() {
	destroyFlag = true;
	for (auto& p : points) {
		p.draw(' ');
	}
	points.clear();
}
