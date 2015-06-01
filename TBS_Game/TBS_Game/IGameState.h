#ifndef IGAMESTATE_H
#define IGAMESTATE_H

#include "IStateBasedGame.h"
class IStateBasedGame;

class IGameState
{
public:
	virtual void init() = 0;
	virtual void cleanup() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void handleEvents(IStateBasedGame& game) = 0;
	virtual void update(IStateBasedGame& game) = 0;
	virtual void draw(IStateBasedGame& game) = 0;
};

#endif