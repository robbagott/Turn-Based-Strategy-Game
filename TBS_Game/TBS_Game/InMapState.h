#ifndef INMAPSTATE_H
#define INMAPSTATE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "IGameState.h"
#include "MapTile.h"

class InMapState : public IGameState
{
public:
	InMapState(Game& game, std::string filename);
	~InMapState();
	void init();
	void cleanup();

	void pause();
	void resume();

	void handleEvents(Game& game);
	void update(Game& game);
	void draw(Game& game);

private:
	InMapState();
	InMapState(const InMapState& otherState);
	void operator=(const InMapState& otherState);

	void moveSelected(int x, int y);

	int m_selectedx;
	int m_selectedy;
	std::vector<std::vector<MapTile>> m_tiles;

	sf::Texture m_cursorTexture;
	sf::Sprite m_cursorOverlay;

	sf::Music m_music;
};

#endif