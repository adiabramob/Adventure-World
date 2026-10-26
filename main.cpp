#include "Menu.h"
#include "Utils.h"
#include "GameReplay.h"

int main(int argc, char* argv[]) {
	std::string mode = "simple";
	bool silent = false;

	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "-save") {
			mode = "save";
		}
		else if (arg == "-load") {
			mode = "load";
		}
		else if (arg == "-silent") {
			silent = true;
		}
	}

	if (mode == "save") {
		Menu menu(true);
		menu.start();
	}
	else if (mode == "load") {
		std::ifstream fileCheck("adv-world.steps");
		if (!fileCheck) {
			std::cout << "Error: 'adv-world.steps' not found in current directory!" << std::endl;
			return 1;
		}
		fileCheck.close();

		if (!silent) {
			std::cout << "Loading recorded game..." << std::endl;
		}

		GameReplay replay(silent);
		replay.start();
	}
	else {
		Menu menu(false);
		menu.start();
	}

	return 0;
}
