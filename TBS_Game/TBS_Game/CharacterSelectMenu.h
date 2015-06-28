#ifndef SELECTMENU_H
#define SELECTMENU_H

#include <SFML/Graphics.hpp>
#include "ICharacter.h"
#include "SpriteSheet.h"

class InMapState;

class CharacterSelectMenu 
{
public:
	CharacterSelectMenu(ICharacter& character, InMapState& mapState);
	~CharacterSelectMenu();

	enum Option {CSM_ATTACK, CSM_WAIT, CSM_SIZE};

	//Returns an option if an option is chosen.
	//If nothing is chosen, it returns CSM_SIZE
	Option handleEvent(sf::Event event);
	void update();
	void draw(sf::RenderWindow& window);

private:
	CharacterSelectMenu(const CharacterSelectMenu& other);
	void operator=(const CharacterSelectMenu& other);

	void select(Option choice);

	InMapState& m_mapState;
	ICharacter& m_character;
	std::vector<SpriteSheet> m_options;
	SpriteSheet m_cursor;
	Option m_selected;
};

#endif