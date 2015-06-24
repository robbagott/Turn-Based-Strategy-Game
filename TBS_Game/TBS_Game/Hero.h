#ifndef HERO_H
#define HERO_H

#include "ICharacter.h"
#include <SFML/Graphics.hpp>
#include "SpriteSheet.h"
#include "CharacterOverlay.h"

class Hero : public ICharacter
{
public:
	Hero(IGameState& gameState, std::string heroName, bool friendly);
	Hero(const Hero& other);
	~Hero();
	void operator=(const Hero& other);

	void handleEvent(sf::Event event);
	void update();
	void draw(sf::RenderWindow& window);


	sf::Vector2f position() const;
	void setPosition(const int& x, const int& y);
	bool friendly() const;
	const std::string& name() const;

private:
	
	IGameState& m_gameState;
	SpriteSheet m_spriteSheet;
	CharacterOverlay m_overlay;
	std::string m_name;
};

#endif
