#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class MenuButton
{
public:
	MenuButton(std::string name, std::string selectedFilename, std::string unselectedFilename, int left, int top, int height, int width);

	std::string name() const;
	const sf::Vector2f& position() const;
	sf::FloatRect bounds() const;
	void select();
	void silentSelect();
	void deselect();
	bool isSelected() const;

	void draw(sf::RenderWindow& window);

private:
	sf::Texture m_selectedTexture;
	sf::Texture m_unselectedTexture;
	sf::Sprite m_sprite;
	sf::Sound m_sound;
	sf::SoundBuffer m_selectSound;
	bool m_isSelected;
	std::string m_name;
};

#endif