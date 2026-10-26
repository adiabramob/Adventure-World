#include "Game.h"
#include "Point.h"
#include "Game.h"
#include <iostream>
#include <fstream>
#include <sstream>

bool Game::useColors = true;

Game::Game() : player1(1, 5, '$'), player2(2, 5, '&')
{
	screen.loadScreensFromFiles();
	if (!loadRiddlesFromFile("riddles.txt")) {
		std::cout << "Failed to load riddles from file." << std::endl;
		exit(0);
	}

	screen.loadScreensFromFiles();


	loadHints("hints.txt");

	std::vector<std::string> filenames = { "adv-world_01.screen.txt", "adv-world_02.screen.txt", "adv-world_03.screen.txt", "adv-world_04.screen.txt" };

	for (int i = 0; i < filenames.size(); i++) {
		std::ifstream file(filenames[i]);
		if (!file.is_open()) continue;

		auto checkBoundsAndFix = [&](int& x, int& y, std::string objName) {
			if (!screen.isValidPosition(x, y)) {
				system("cls");
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << "WARNING: Coordinate Error in file: " << filenames[i] << std::endl;
				std::cout << "Object '" << objName << "' was out of bounds at (" << x << ", " << y << ")" << std::endl;

				x = 8;
				y = 7;

				std::cout << "It has been moved to default safe position: (8, 7)" << std::endl;
				std::cout << "Please fix the file coordinates for future runs." << std::endl;
				std::cout << "\nPress any key to continue loading..." << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;

				(void)_getch();
				system("cls");
			}
			};

		std::string line;
		for (int j = 0; j < 25; ++j) std::getline(file, line);

		while (std::getline(file, line)) {
			if (line.empty()) continue;

			std::stringstream ss(line);
			char type;
			if (!(ss >> type)) continue;

			if (type >= '1' && type <= '9') {
				int x, y, r, t;
				if (ss >> x >> y >> r >> t) {
					checkBoundsAndFix(x, y, "Door " + std::string(1, type));
					doors.push_back(Door(x, y, (Door::DoorID)type, r, t));
				}
			}
			else if (type == 'K') {
				int x, y, r; char kID;
				if (ss >> x >> y >> kID >> r) {
					checkBoundsAndFix(x, y, "Key " + std::string(1, kID));
					keys.push_back(Key(x, y, (Key::KeyID)kID, r));
				}
			}
			else if (type == '/' || type == '\\') {
				int x, y, r, sID;
				if (ss >> x >> y >> r >> sID) {
					checkBoundsAndFix(x, y, "Switch");
					switches.push_back(Switch(x, y, r, sID));
				}
			}
			else if (type == '*') {
				int numPoints, r;
				if (ss >> numPoints >> r) {
					std::vector<Point> pts;
					for (int p = 0; p < numPoints; ++p) {
						int px, py;
						if (ss >> px >> py) {
							checkBoundsAndFix(px, py, "Obstacle Point");
							pts.push_back(Point(px, py, 0, 0, '*'));
						}
					}
					obstacles.push_back(Obstacle(pts, r));
				}
			}
			else if (type == '#') {
				int numPoints, r;
				if (ss >> numPoints >> r) {
					std::vector<Point> pts;
					for (int p = 0; p < numPoints; ++p) {
						int px, py;
						if (ss >> px >> py) {
							checkBoundsAndFix(px, py, "Spring Point");
							pts.push_back(Point(px, py, 0, 0, '#'));
						}
					}
					springs.push_back(Spring(pts, r, true, '#'));
				}
			}
			else if (type == '!') {
				int x, y, rad, r;
				if (ss >> x >> y >> rad >> r) {
					checkBoundsAndFix(x, y, "Torch");
					torches.push_back(Torch(x, y, rad, r));
				}
			}
			else if (type == '@') {
				int x, y, r;
				if (ss >> x >> y >> r) {
					checkBoundsAndFix(x, y, "Bomb");
					bombs.push_back(Bomb(x, y, r));
				}
			}
			else if (type == 'D') {
				int isDarkRoom;
				if (ss >> isDarkRoom) {
					screen.setDark(i, isDarkRoom == 1);
				}
			}
		}
		file.close();
	}

	if (doors.size() > 1) {
		doors[1].linkSwitch(201);
		doors[1].linkSwitch(202);
	}
}

void Game::transferSpring(Player& from, Player& to) {
	to.setSpeed(from.getSpeed());
	to.setSpringBoostTime(from.getSpringBoostTime());
	to.setDirection(from.getCurrentDirection());

	from.setSpeed(1);
	from.setSpringBoostTime(0);
	from.setDirection(Direction::STAY);
}

//from chat gpt
void Game::update() {
	int livesBeforeP1 = player1.getLives();
	int livesBeforeP2 = player2.getLives();
	player1.erase();
	player2.erase();

	for (Key& k : keys) {
		if (!k.isPickedUp() && k.getRoomID() == screen.getCurrentScreenID()) {
			if (player1.getX() == k.getX() && player1.getY() == k.getY()) {
				player1.pickHaldItem(k.getIDchar());
				k.pickUp();
			}
			else if (player2.getX() == k.getX() && player2.getY() == k.getY()) {
				player2.pickHaldItem(k.getIDchar());
				k.pickUp();
			}
		}
	}

	for (Torch& t : torches) {
		if (!t.isPicked() && t.getRoomID() == screen.getCurrentScreenID()) {
			if (player1.getX() == t.getX() && player1.getY() == t.getY()) {
				player1.pickHaldItem('T');
				t.pickUp();
			}
			else if (player2.getX() == t.getX() && player2.getY() == t.getY()) {
				player2.pickHaldItem('T');
				t.pickUp();
			}
		}
	}

	for (Bomb& b : bombs) {
		if (b.getRoomID() != screen.getCurrentScreenID())
			continue;

		if (!b.pickedUp()) {
			if (player1.getX() == b.getX() && player1.getY() == b.getY() && player1.getHaldItem() == ' ') {
				player1.pickHaldItem('@');
				b.pickUp();
			}
			else if (player2.getX() == b.getX() && player2.getY() == b.getY() && player2.getHaldItem() == ' ') {
				player2.pickHaldItem('@');
				b.pickUp();
			}
		}

		if (player1.getHaldItem() == '@' && player1.getCurrentDirection() == Direction::STAY) {
			b.release(player1, screen);
			player1.dropHaldItem();
		}
		if (player2.getHaldItem() == '@' && player2.getCurrentDirection() == Direction::STAY) {
			b.release(player2, screen);
			player2.dropHaldItem();
		}

		b.updateBomb(player1, player2, doors, switches, keys, springs, riddles, obstacles, hints, screen);

		if (!b.exploded() && b.active()) {
			b.draw();
		}
	}
	checkPlayersLives();
	if (!isRunning) return;


	std::vector<Point> openDoors;
	for (Door& d : doors) {
		if (d.getRoomID() == screen.getCurrentScreenID() && d.isOpenNow({ player1, player2 }, switches)) {
			openDoors.push_back(Point(d.getX(), d.getY(), 0, 0, d.getID()));
		}
	}

	for (Spring& s : springs) {
		if (s.getRoomID() == screen.getCurrentScreenID()) {
			int stepsP1 = s.activate(player1, screen);
			if (stepsP1 > 0) {
				player1.setSpeed(stepsP1);
				player1.setSpringBoostTime(stepsP1 * stepsP1);
			}

			int stepsP2 = s.activate(player2, screen);
			if (stepsP2 > 0) {
				player2.setSpeed(stepsP2);
				player2.setSpringBoostTime(stepsP2 * stepsP2);
			}
		}
	}


	handleRiddle(player1);
	handleRiddle(player2);
	handleHint(player1);
	handleHint(player2);

	auto movePlayer = [&](Player& p) {
		if (p.getSpeed() > 1) {
			p.moveWithSpeed(screen, doors, { player1, player2 }, obstacles, switches, keys, riddles, hints, *this);

			p.decreaseSpringBoostTime();
			if (p.getSpringBoostTime() == 0) {
				p.setSpeed(1);
				for (Spring& s : springs) {
					if (s.getRoomID() == screen.getCurrentScreenID()) {
						s.reset();
					}
				}
			}
		}
		else {
			if (p.canMove(screen, doors, { player1, player2 }, obstacles, switches))
				p.move(screen);
		}
		};

	movePlayer(player1);
	movePlayer(player2);

	if (player1.getX() == player2.getX() &&
		player1.getY() == player2.getY()) {

		if (player1.getSpringBoostTime() > 0 && player2.getSpringBoostTime() == 0) {
			transferSpring(player1, player2);
		}
		else if (player2.getSpringBoostTime() > 0 && player1.getSpringBoostTime() == 0) {
			transferSpring(player2, player1);
		}
	}


	for (Obstacle& o : obstacles) {
		if (o.getRoomID() == screen.getCurrentScreenID()) {
			o.tryMove(player1.getCurrentDirection(), player1, player2, screen);
		}
	}


	if (player1.getCurrentDirection() == Direction::STAY) {
		for (const Obstacle& o : obstacles) {
			o.releasePlayer(player1, screen);
		}
	}
	if (player2.getCurrentDirection() == Direction::STAY) {
		for (const Obstacle& o : obstacles) {
			o.releasePlayer(player2, screen);
		}
	}

	for (Switch& s : switches) {
		if (s.getRoomID() == screen.getCurrentScreenID()) {
			if (player1.getX() == s.getX() && player1.getY() == s.getY()) {
				if (!s.getState()) {
					s.toggle();
				}
			}
			if (player2.getX() == s.getX() && player2.getY() == s.getY()) {
				if (!s.getState()) {
					s.toggle();
				}
			}
		}
	}
	if (player1.getLives() < livesBeforeP1) {
		logEvent("PLAYER_LOST_LIFE | Player 1");
	}
	if (player2.getLives() < livesBeforeP2) {
		logEvent("PLAYER_LOST_LIFE | Player 2");
	}
}

void Game::draw() {
	for (const Key& k : keys) {
		if (!k.isPickedUp() && k.getRoomID() == screen.getCurrentScreenID()) {
			k.draw();
		}
	}

	for (const Riddle& r : riddles) {
		if (!r.isSolved() && !r.destroyed() && r.getRoomID() == screen.getCurrentScreenID()) {
			r.draw();
		}
	}

	for (const Obstacle& o : obstacles) {
		if (o.getRoomID() == screen.getCurrentScreenID()) {
			o.draw();
		}
	}

	player1.draw();
	player2.draw();

	for (const Switch& s : switches) {
		if (s.getRoomID() == screen.getCurrentScreenID()) {
			s.draw();
		}
	}

	for (const Door& d : doors) {
		if (!d.destroyed() && d.getRoomID() == screen.getCurrentScreenID()) {
			if (!d.getIsOpen()) {
				gotoxy(d.getX(), d.getY());
				Bonus::setColorByType((char)d.getID());
				std::cout << (char)d.getID();
				Bonus::resetColor();
			}
		}
	}

	for (Spring& s : springs) {
		if (!s.destroyed() && s.getRoomID() == screen.getCurrentScreenID()) {
			s.draw();
		}
	}

	if (screen.isDark(screen.getCurrentScreenID())) {
		drawDarkRoomWithTorch();
		return;
	}

	for (Bomb& b : bombs) {
		if (b.getRoomID() == screen.getCurrentScreenID() && !b.exploded()) {
			b.draw();
		}
	}

	for (const Hint& h : hints) {
		if (!h.destroyed() && h.getRoomID() == screen.getCurrentScreenID()) {
			h.draw();
		}
	}

	HUD::draw(player1, player2, screen.getLegendX(), screen.getLegendY());

}

Door* Game::getDoorAt(int x, int y) {
	for (Door& d : doors)
		if (d.getX() == x && d.getY() == y &&
			d.getRoomID() == screen.getCurrentScreenID())
			return &d;
	return nullptr;
}

void Game::drawDarkRoom() {
	for (int y = 0; y < Screen::MAX_Y; y++) {
		for (int x = 0; x < Screen::MAX_X; x++) {
			gotoxy(x, y);
			std::cout << ' ';
		}
	}
}

//from chatgpt
void Game::drawDarkRoomWithTorch() {
	static char buffer[Screen::MAX_Y][Screen::MAX_X] = { {' '} };
	char newBuffer[Screen::MAX_Y][Screen::MAX_X];

	for (int y = 0; y < Screen::MAX_Y; y++)
		for (int x = 0; x < Screen::MAX_X; x++)
			newBuffer[y][x] = ' ';

	int radius = 10;
	int currentRoom = screen.getCurrentScreenID();

	Player* players[2] = { &player1, &player2 };
	for (Player* player : players) {
		if (player->getHaldItem() != 'T') continue;

		int px = player->getX();
		int py = player->getY();

		for (int y = py - radius; y <= py + radius; y++) {
			for (int x = px - radius; x <= px + radius; x++) {
				if (x < 0 || x >= Screen::MAX_X || y < 0 || y >= Screen::MAX_Y)
					continue;

				int dx = x - px;
				int dy = y - py;

				if (dx * dx + dy * dy <= radius * radius) {
					char c = screen.getScreenChar(x, y);
					if (c == 'w' || (c >= '1' && c <= '3')) newBuffer[y][x] = c;

					for (const Key& k : keys) {
						if (!k.isPickedUp() && k.getRoomID() == currentRoom && k.getX() == x && k.getY() == y) {
							newBuffer[y][x] = 'K';
							break;
						}
					}

					for (const Riddle& r : riddles) {
						if (!r.isSolved() && r.getRoomID() == currentRoom && r.getX() == x && r.getY() == y) {
							newBuffer[y][x] = '?';
							break;
						}
					}

					for (const Door& d : doors) {
						if (d.getRoomID() == currentRoom && !d.getIsOpen() && d.getX() == x && d.getY() == y) {
							newBuffer[y][x] = (char)d.getID();
							break;
						}
					}

					for (const Hint& h : hints) {
						if (!h.destroyed() && h.getRoomID() == currentRoom && h.getX() == x && h.getY() == y) {
							newBuffer[y][x] = 'T';
							break;
						}
					}

					for (const Bomb& b : bombs) {
						if (!b.exploded() && b.getRoomID() == currentRoom && b.getX() == x && b.getY() == y) {
							newBuffer[y][x] = '@';
							break;
						}
					}

					for (const Spring& s : springs) {
						if (!s.destroyed() && s.getRoomID() == currentRoom && s.contains(x, y)) {
							newBuffer[y][x] = '#';
							break;
						}
					}

					for (const Switch& s : switches) {
						if (s.getRoomID() == currentRoom && s.getX() == x && s.getY() == y) {
							newBuffer[y][x] = (s.getState() ? '\\' : '/');
							break;
						}
					}
				}
			}
		}
	}

	for (const Torch& t : torches) {
		if (!t.isPicked() && t.getRoomID() == currentRoom)
			newBuffer[t.getY()][t.getX()] = '!';
	}

	newBuffer[player1.getY()][player1.getX()] = player1.getSymbol();
	newBuffer[player2.getY()][player2.getX()] = player2.getSymbol();

	for (int y = 0; y < Screen::MAX_Y; y++) {
		for (int x = 0; x < Screen::MAX_X; x++) {
			if (newBuffer[y][x] != buffer[y][x]) {
				gotoxy(x, y);
				Bonus::setColorByType(newBuffer[y][x]);
				std::cout << newBuffer[y][x];
				Bonus::resetColor();
				buffer[y][x] = newBuffer[y][x];
			}
		}
	}
}

void Game::checkRoomTransition() {
	Door* doorInRoom = nullptr;

	for (Door& d : doors) {
		if (d.getRoomID() == screen.getCurrentScreenID() && d.isOpenNow({ player1, player2 }, switches)) {
			doorInRoom = &d;
			break;
		}
	}

	if (!doorInRoom) return;

	bool p1OnDoor = (player1.getX() == doorInRoom->getX() && player1.getY() == doorInRoom->getY());
	bool p2OnDoor = (player2.getX() == doorInRoom->getX() && player2.getY() == doorInRoom->getY());

	if (p1OnDoor && player1.getCurrentDirection() != Direction::STAY) player1.setDirection(Direction::STAY);
	if (p2OnDoor && player2.getCurrentDirection() != Direction::STAY) player2.setDirection(Direction::STAY);

	if (p1OnDoor && p2OnDoor) {
		player1.dropHaldItem();
		player2.dropHaldItem();

		if (screen.getCurrentScreenID() == 3) {
			logEvent("GAME_ENDED | SUCCESS | Final Score: " + std::to_string(player1.getScore() + player2.getScore()));
			isRunning = false;
			return;
		}

		logEvent("LEVEL_PASSED_TO_" + std::to_string(doorInRoom->getTarget()));

		screen.nextScreen(doorInRoom->getTarget());
		player1.setPosition(1, 5);
		player2.setPosition(2, 5);
		player1.setDirection(Direction::STAY);
		player2.setDirection(Direction::STAY);


		screen.draw();
		player1.draw();
		player2.draw();
	}
}

bool Game::isOutOfBounds(const Point& point) const {
	return point.getX() < 0 || point.getX() >= Screen::MAX_X || point.getY() < 0 || point.getY() >= Screen::MAX_Y;
}

void Game::handleRiddle(Player& player) {
	int nx = player.getX();
	int ny = player.getY();

	switch (player.getCurrentDirection()) {
	case Direction::UP:
		ny -= 1;
		break;
	case Direction::DOWN:
		ny += 1;
		break;
	case Direction::LEFT:
		nx -= 1;
		break;
	case Direction::RIGHT:
		nx += 1;
		break;
	default: break;
	}

	for (Riddle& r : riddles) {
		if (!r.isSolved() && r.getX() == nx && r.getY() == ny && r.getRoomID() == screen.getCurrentScreenID()) {

			player.setDirection(Direction::STAY);

			cls();
			std::cout << "Riddle: " << r.getQuestion() << std::endl;
			std::cout << "\nYour answer: ";

			std::string answer;
			std::cin >> answer;

			if (answer == r.getAnswer()) {
				logEvent("RIDDLE_SOLVED");
				std::cout << "Correct!" << std::endl;
				r.solve();
				player.addScore(10);
				Sleep(700);
				player.forceMoveTo(r.getX(), r.getY());
			}
			else {
				logEvent("RIDDLE_FAILED");
				std::cout << "Wrong! You stay here... \n";
				player.subtractScore(10);
				Sleep(700);

				if (player.getScore() <= 0) {
					cls();
					std::cout << "GAME OVER !" << std::endl;
					Sleep(1500);
					isRunning = false;
				}
			}
			cls();
			screen.draw();
			player1.draw();
			player2.draw();

			return;
		}
	}
}

void Game::handleHint(Player& player) {
	int nx = player.getX();
	int ny = player.getY();

	switch (player.getCurrentDirection()) {
	case Direction::UP:
		ny -= 1;
		break;
	case Direction::DOWN:
		ny += 1;
		break;
	case Direction::LEFT:
		nx -= 1;
		break;
	case Direction::RIGHT:
		nx += 1;
		break;
	default: break;
	}

	for (Hint& h : hints) {
		if (!h.destroyed() && h.getX() == nx && h.getY() == ny && h.getRoomID() == screen.getCurrentScreenID()) {

			player.setDirection(Direction::STAY);

			cls();

			std::cout << "============================" << std::endl;
			std::cout << "       HINT FOUND!          " << std::endl;
			std::cout << "============================" << std::endl;
			std::cout << "\nMessage: " << h.getMessage() << std::endl;

			std::cout << "\n\nPress any key to continue...";

			while (_kbhit()) _getch();

			_getch();

			cls();
			screen.draw();

			for (const Hint& hintObj : hints) {
				if (!hintObj.destroyed() && hintObj.getRoomID() == screen.getCurrentScreenID()) {
					hintObj.draw();
				}
			}

			player1.draw();
			player2.draw();

			return;
		}
	}
}

//from chat gpt
void Game::start() {
	hideCursor();
	screen.loadScreensFromFiles();
	screen.draw();
	player1.draw();
	player2.draw();
	while (isRunning) {
		handleInput();
		update();
		draw();
		checkRoomTransition();
		gameCycle++;
		Sleep(50);
	}
	cls();
	std::cout << "Game Over!" << std::endl;
}

//from chat gpt
void Game::executeCommand(int ch) {
	Keys key = static_cast<Keys>(ch);

	switch (key) {
	case Keys::UP_P1:
		player1.setDirection(Direction::UP);
		break;
	case Keys::DOWN_P1:
		player1.setDirection(Direction::DOWN);
		break;
	case Keys::LEFT_P1:
		player1.setDirection(Direction::LEFT);
		break;
	case Keys::RIGHT_P1:
		player1.setDirection(Direction::RIGHT);
		break;
	case Keys::STAY_P1:
		player1.setDirection(Direction::STAY);
		break;
	case Keys::UP_P2:
		player2.setDirection(Direction::UP);
		break;
	case Keys::DOWN_P2:
		player2.setDirection(Direction::DOWN);
		break;
	case Keys::LEFT_P2:
		player2.setDirection(Direction::LEFT);
		break;
	case Keys::RIGHT_P2:
		player2.setDirection(Direction::RIGHT);
		break;
	case Keys::STAY_P2:
		player2.setDirection(Direction::STAY);
		break;
	case Keys::ESC:
		pauseGame();
		break;
	default:
		break;
	}
}

void Game::handleInput() {
	if (_kbhit()) {
		int ch = _getch();
		executeCommand(ch);
	}
}

//from copilot
void Game::pauseGame() {
	cls();
	std::cout << "Game Paused" << std::endl;
	std::cout << "Press ESC to continue..." << std::endl;
	std::cout << "Press H to return to main menu..." << std::endl;

	bool paused = true;
	while (paused) {
		if (_kbhit()) {
			int ch = _getch();
			Keys key = static_cast<Keys>(ch);

			switch (key) {
			case Keys::ESC:
				paused = false;
				break;
			case Keys::BACK_TO_MENU_L:
			case Keys::BACK_TO_MENU_U:
				isRunning = false;
				paused = false;
				break;
			default:
				break;
			}
		}
		Sleep(50);
	}
	screen.draw();
	player1.draw();
	player2.draw();
}

void Game::checkPlayersLives() {
	if (player1.getLives() <= 0 || player2.getLives() <= 0) {

		std::string scoreMsg = "GAME_OVER_NO_LIVES | P1_SCORE: " + std::to_string(player1.getScore())
			+ " | P2_SCORE: " + std::to_string(player2.getScore());

		logEvent(scoreMsg);

		cls();
		std::cout << "GAME OVER !" << std::endl;
		std::cout << "Player 1 Score: " << player1.getScore() << std::endl;
		std::cout << "Player 2 Score: " << player2.getScore() << std::endl;

		Sleep(2500);
		isRunning = false;
	}
}

void Game::handlePlayerCollisions(Player& p, Player& other) {
	int dx = 0, dy = 0;

	switch (p.getCurrentDirection()) {
	case Direction::UP:    dy = -1; break;
	case Direction::DOWN:  dy = 1;  break;
	case Direction::LEFT:  dx = -1; break;
	case Direction::RIGHT: dx = 1;  break;
	case Direction::STAY:
		if (p.getSpringBoostTime() > 0)
			break;
		return;
	}

	for (int step = 0; step < p.getSpeed(); ++step) {
		int nx = p.getX() + dx;
		int ny = p.getY() + dy;

		if (nx == other.getX() && ny == other.getY()) {
			if (p.getSpringBoostTime() > 0 && other.getSpringBoostTime() == 0) {
				transferSpring(p, other);
			}
			return;
		}

		p.erase();
		p.setPosition(nx, ny);
		p.draw();
	}
}

bool Game::loadRiddlesFromFile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cout << "Cannot open riddles file: " << filename << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#')
			continue;

		std::stringstream ss(line);
		std::string posPart, question, answer;

		std::getline(ss, posPart, '|');
		std::getline(ss, question, '|');
		std::getline(ss, answer);

		std::stringstream posSS(posPart);
		std::string xStr, yStr, roomStr;

		std::getline(posSS, xStr, ',');
		std::getline(posSS, yStr, ',');
		std::getline(posSS, roomStr, ',');

		int x = std::stoi(xStr);
		int y = std::stoi(yStr);
		int room = std::stoi(roomStr);

		riddles.emplace_back(x, y, question, answer, room);
	}

	return true;
}

void Game::loadHints(const std::string& filename) {
	std::ifstream file(filename);
	std::string line;
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') continue;

		size_t pipePos = line.find('|');
		std::string posPart = line.substr(0, pipePos);
		std::string msg = line.substr(pipePos + 1);

		std::stringstream ss(posPart);
		int x, y, room;
		char comma;
		ss >> x >> comma >> y >> comma >> room;

		hints.emplace_back(x, y, msg, room);
	}
}