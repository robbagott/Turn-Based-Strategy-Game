#ifndef INMAPSTATE_H
#define INMAPSTATE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "IGameState.h"
#include "MapTile.h"
#include "SpriteSheet.h"
#include "ICharacter.h"
#include "CharacterSelectMenu.h"

class InMapState : public IGameState
{
public:
	InMapState(Game& game, std::string filename);
	~InMapState();

	enum MapState {MS_DEFAULT, MS_CHARACTERSELECTED, MS_CHARACTERPLACED, MS_CHARACTERATTACKING, MS_ANIMATIONPLAYING};
	enum Turn {PLAYERTURN, ENEMYTURN};

	void init();
	void cleanup();

	void pause();
	void resume();

	void handleEvents();
	void update();
	void draw();

	ICharacter* characterAt(const int& x, const int& y);

private:
	//Don't copy or incorrectly construct
	InMapState();
	InMapState(const InMapState& otherState);
	void operator=(const InMapState& otherState);

	///implementation functions for clarity of code
	void playerControl();
	void aiControl();

	void moveSelected(const unsigned int& x,const unsigned int& y);
	void onSelectPress();
	void populateMoveSpan(int movePoints, const int& x, const int& y);
	
	Game& m_game;

	//Map stuff
	std::vector<std::vector<MapTile>> m_tiles;
	sf::Vector2i m_focalTile;
	sf::Vector2i m_selected;

	//Character stuff
	std::vector<sf::Vector2i> m_moveSpan;
	std::vector<sf::Vector2i> m_attackSpan;
	std::vector<ICharacter*> m_characters;
	ICharacter* m_selectedCharacter;

	//Assets
	SpriteSheet m_cursor;
	SpriteSheet m_moveSpanSprite;
	sf::Music m_music;

	//Logic stuff
	MapState m_mapState;
	Turn m_turn;

	//Overlay stuff
	std::vector<CharacterSelectMenu*> m_menus;
};

#endif