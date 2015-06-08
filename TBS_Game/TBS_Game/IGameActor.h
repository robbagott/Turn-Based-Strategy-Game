#ifndef GAMEACTOR_H
#define GAMEACTOR_H

#include "IGameState.h"

class IGameActor
{
public:
	virtual ~IGameActor() {}
	virtual void handleEvent(IGameState& gameState, sf::Event event) = 0;
	virtual void update(IGameState& gameState) = 0;
	virtual void draw(IGameState& gameState, sf::RenderWindow& window) = 0;

protected:

};

#endif