#ifndef INMAPSTATE_H
#define INMAPSTATE_H

#include "IGameState.h"
#include "MapTile.h"

class IStateBasedGame;

class InMapState : public IGameState
{
public:
	InMapState(std::string filename);
	~InMapState();
	void init();
	void cleanup();

	void pause();
	void resume();

	void handleEvents(IStateBasedGame& game);
	void update(IStateBasedGame& game);
	void draw(IStateBasedGame& game);

private:
	InMapState();

	std::vector<std::vector<MapTile>> tiles;
};

#endif