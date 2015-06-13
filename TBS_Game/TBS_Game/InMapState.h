#ifndef INMAPSTATE_H
#define INMAPSTATE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "IGameState.h"
#include "MapTile.h"
#include "SpriteSheet.h"
#include "ICharacter.h"

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

	ICharacter* characterAt(const int& x, const int& y);

private:
	InMapState();
	InMapState(const InMapState& otherState);
	void operator=(const InMapState& otherState);

	void moveSelected(const unsigned int& x,const unsigned int& y);
	void onSelectPress();
	
	Game& m_game;
	std::vector<std::vector<MapTile>> m_tiles;
	sf::Vector2i m_focalTile;
	sf::Vector2i m_selected;
	
	std::vector<ICharacter*> m_characters;
	ICharacter* m_selectedCharacter;
	SpriteSheet m_cursor;
	sf::Music m_music;

	bool m_characterIsSelected;
};

#endif