#include "Door.h"

bool Door::canOpen(const std::vector<Switch>& switches) const {
	for (int sid : linkedSwitches) {
		for (const Switch& sw : switches) {
			if (sw.getID() == sid && !sw.getState()) {
				return false;
			}
		}
	}
	return true;
}

bool Door::isOpenNow(const std::vector<Player>& players, const std::vector<Switch>& switches) const {
	if (!linkedSwitches.empty() && canOpen(switches)) {
		return true;
	}

	for (const Player& p : players) {
		if (p.getHaldItem() == getID()) {
			return true;
		}
	}

	return false;
}
