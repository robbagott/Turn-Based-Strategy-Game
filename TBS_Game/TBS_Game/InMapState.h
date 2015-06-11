#ifndef INMAPSTATE_H
#define INMAPSTATE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "IGameState.h"
#include "MapTile.h"
#include "SpriteSheet.h"

class InMapState : public IGameState
{
public:
	InMapState(Game& game, std::string filename);
	~InMapState();
	void init();
	void cleanup();

	void pause();
	void resume();

	void handleEvents();
	void update();
	void draw();

private:
	InMapState();
	InMapState(const InMapState& otherState);
	void operator=(const InMapState& otherState);

	void moveSelected(int x, int y);

	Game& m_game;
	int m_selectedx;
	int m_selectedy;
	std::vector<std::vector<MapTile>> m_tiles;

	SpriteSheet m_cursor;

	sf::Music m_music;
};

#endif