#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <SFML/Graphics.hpp>

class MenuButton
{
public:
	MenuButton(std::string selectedFilename, std::string unselectedFilename, int left, int top, int height, int width);

	int left() const;
	int top() const;
	int height() const;
	int width() const;
	int bottom() const;
	int right() const;

private:
	int m_left, m_top, m_height, m_width;
	sf::Texture m_selectedTexture;
	sf::Texture m_unselectedTexture;
	sf::Sprite m_sprite;
};

#endif