#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "SpriteSheet.h"

class MenuButton
{
public:
	MenuButton(std::string spriteSheetName, int left, int top, int height, int width);
	MenuButton(const MenuButton&);
	~MenuButton();
	void operator=(const MenuButton&);
	sf::Vector2f position();
	sf::FloatRect bounds();
	void select();
	void silentSelect();
	void deselect();
	bool isSelected() const;

	void update();
	void draw(sf::RenderWindow& window);

private:
	//undefined
	MenuButton();

	SpriteSheet m_spriteSheet;
	sf::Sound m_sound;
	sf::SoundBuffer m_selectSound;
	bool m_isSelected;
};

#endif