#ifndef IGAMESTATE_H
#define IGAMESTATE_H

#include "Game.h"

class IGameState
{
public:
	virtual ~IGameState() {}
	virtual void init() = 0;
	virtual void cleanup() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void handleEvents() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
};

#endif