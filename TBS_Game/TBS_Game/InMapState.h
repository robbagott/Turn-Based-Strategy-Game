#ifndef INMAPSTATE_H
#define INMAPSTATE_H

#include <SFML/Graphics.hpp>
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

	void moveSelected(int x, int y);

	int m_selectedx;
	int m_selectedy;
	std::vector<std::vector<MapTile>> m_tiles;

	sf::Texture m_cursorTexture;
	sf::Sprite m_cursorOverlay;
};

#endif