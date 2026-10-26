#include "Torch.h"

void Torch::draw() {
	if (!picked) {
		gotoxy(position.getX(), position.getY());
		std::cout << position.getSymbol();
	}
}
