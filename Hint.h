#pragma once
#include "Point.h"
#include <string>
#include <vector>

class Hint {
    Point position;
    std::string message;
    int roomID;
    bool destroyFlag = false;
public:
    Hint(int x, int y, const std::string& msg, int room)
        : position(x, y, 0, 0, 'T'), message(msg), roomID(room) {
    }

    int getX() const { return position.getX(); }
    int getY() const { return position.getY(); }
    int getRoomID() const { return roomID; }
    const std::string& getMessage() const { return message; }

    void draw() const {
        if (!destroyFlag) {
            const_cast<Point&>(position).draw(position.getSymbol());
        }
    }

    void destroy() {
        destroyFlag = true;
        position.draw(' ');
    }

    bool destroyed() const { return destroyFlag; }

    std::vector<Point> getPoints() const { return { position }; }
};
