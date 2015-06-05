#ifndef NULLGAMESTATE_H
#define NULLGAMESTATE_H

#include "IGameState.h"

class NullGameState : public IGameState
{
public:
	NullGameState(Game& game) : m_game(game) {}
	void init() {}
	void cleanup() {}

	void pause() {}
	void resume() {}

	void handleEvents() { m_game.requestQuit(); }
	void update() {}
	void draw() {}

private:
	NullGameState(const NullGameState&);
	void operator=(const NullGameState&);

	Game& m_game;
};
#endif