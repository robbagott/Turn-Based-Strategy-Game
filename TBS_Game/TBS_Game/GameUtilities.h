#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
namespace GameUtilities {

	//returns the interpolated value between x and y 
	float interpolate(float x, float y, float percentage);

	void exitWithMessage(std::string message);
}

#endif