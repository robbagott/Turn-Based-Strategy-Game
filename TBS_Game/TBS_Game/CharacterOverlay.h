#ifndef CHARACTERPROFILE_H
#define CHARACTERPROFILE_H

#include "TextureManager.h"

class CharacterOverlay
{
public:
	CharacterOverlay();
	CharacterOverlay(const std::string& characterName, const int& currentHealth, const int& totalHealth);
	~CharacterOverlay();
	CharacterOverlay(const CharacterOverlay& other);
	void operator=(const CharacterOverlay& other);

	void draw(sf::RenderWindow& window);
	void setHealth(const int& newHealth);
	void setCharacter(const std::string& characterName, const int& currentHealth, const int& totalHealth);

	void showOverlay(bool show);

private:

	bool m_initialized;
	std::string m_characterName;
	sf::Texture* m_background;
	sf::Texture* m_profileView;
	sf::Sprite m_sprite;

	sf::RectangleShape m_healthBar;
	sf::RectangleShape m_currentHealthBar;
	int m_currentHealth;
	int m_totalHealth;
};

#endif