#include "Game.h"
#include <exception>
#include <iostream>

int main()
//try {
	{
		Game game;
		game.start();
		return 0;
	}
//}
//catch (std::exception& e){
//	std::cerr << e.what() << std::endl;
//	char x;
//	std::cin >> x;
//}