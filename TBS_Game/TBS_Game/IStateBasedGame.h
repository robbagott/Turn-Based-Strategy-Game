#ifndef ISTATEBASEDGAME_H
#define ISTATEBASEDGAME_H

#include <SFML/Graphics.hpp>

#include "IGameState.h"
class IGameState;

class IStateBasedGame
{
public:
	virtual void start() = 0;
	virtual void requestChangeState(IGameState& newState) = 0;
	virtual void requestQuit() = 0;
	virtual sf::RenderWindow* mainWindow() = 0;
};

#endif