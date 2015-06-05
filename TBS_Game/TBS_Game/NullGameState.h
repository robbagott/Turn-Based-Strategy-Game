#ifndef NULLGAMESTATE_H
#define NULLGAMESTATE_H

#include "IGameState.h"

class NullGameState : public IGameState
{
public:
	NullGameState() {}
	void init() {}
	void cleanup() {}

	void pause() {}
	void resume() {}

	void handleEvents(Game& game) { game.requestQuit(); }
	void update(Game& game) {}
	void draw(Game& game) {}

private:
	NullGameState(const NullGameState&);
	void operator=(const NullGameState&);
};
#endif