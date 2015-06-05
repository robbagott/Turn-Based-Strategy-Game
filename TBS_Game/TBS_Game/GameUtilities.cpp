#include "GameUtilities.h"
#include <iostream>
#include <string>

namespace GameUtilities {

	float interpolate(float x, float y, float percentage) {
		if (percentage > 100) {
			percentage = 100;
		}
		else if (percentage < 0) {
			percentage = 0;
		}

		return x + (y - x)*percentage / 100;
	}
	
	void exitWithMessage(std::string message) {
		std::cerr << message << std::endl;
		char x;
		std::cin >> x;
		exit(1);
	}
}