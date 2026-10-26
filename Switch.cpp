#include "Switch.h"
#include <iostream>

void Switch::draw() const {
	gotoxy(x, y);
	if (isOn) {
		std::cout << '\\';
	}
	else {
		std::cout << '/';
	}
}
