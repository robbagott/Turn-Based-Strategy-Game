#ifndef HERO_H
#define HERO_H

#include "ICharacter.h"
#include <SFML/Graphics.hpp>
#include "SpriteSheet.h"

class Hero : public ICharacter
{
public:
	Hero(std::string heroName);
	Hero(const Hero& other);
	~Hero();
	void operator=(const Hero& other);

	void handleEvent(IGameState& gameState, sf::Event event);
	void update(IGameState& gameState);
	void draw(IGameState& gameState, sf::RenderWindow& window);

	sf::Vector2f position() const;
	void setPosition(const int& x, const int& y);

private:
	
	SpriteSheet m_spriteSheet;
	std::string m_name;
};

#endif
