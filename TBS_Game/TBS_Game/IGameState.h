#ifndef IGAMESTATE_H
#define IGAMESTATE_H

#include "Game.h"

class IGameState
{
public:
	virtual void init() = 0;
	virtual void cleanup() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void handleEvents(Game& game) = 0;
	virtual void update(Game& game) = 0;
	virtual void draw(Game& game) = 0;
};

#endif