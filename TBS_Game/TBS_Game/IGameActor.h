#ifndef GAMEACTOR_H
#define GAMEACTOR_H

#include "IGameState.h"

class IGameActor
{
public:
	virtual ~IGameActor() {}
	virtual void handleEvent(sf::Event event) = 0;
	virtual void update() = 0;
	virtual void draw(sf::RenderWindow& window) = 0;

protected:

};

#endif