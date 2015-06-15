#ifndef SELECTMENU_H
#define SELECTMENU_H

#include <SFML/Graphics.hpp>
#include "SpriteSheet.h"

class CharacterSelectMenu 
{
public:
	CharacterSelectMenu();
	~CharacterSelectMenu();

	void handleEvent(sf::Event event);
	void update();
	void draw(sf::RenderWindow& window);

private:
	CharacterSelectMenu(const CharacterSelectMenu& other);
	void operator=(const CharacterSelectMenu& other);

	void select(int choice);

	std::vector<SpriteSheet> m_options;
	SpriteSheet m_cursor;

	int m_selected;
};

#endif