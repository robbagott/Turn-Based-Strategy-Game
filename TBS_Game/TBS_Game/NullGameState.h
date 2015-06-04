#ifndef NULLGAMESTATE_H
#define NULLGAMESTATE_H

#include "IGameState.h"
class IStateBasedGame;

class NullGameState : public IGameState
{
public:
	void init() {}
	void cleanup() {}

	void pause() {}
	void resume() {}

	void handleEvents(IStateBasedGame& game) { game.requestQuit(); }
	void update(IStateBasedGame& game) {}
	void draw(IStateBasedGame& game) {}

private:
	NullGameState(const NullGameState&);
	void operator=(const NullGameState&);
};
#endif