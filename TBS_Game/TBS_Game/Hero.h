#ifndef HERO_H
#define HERO_H

#include "ICharacter.h"
#include <SFML/Graphics.hpp>

class Hero : public ICharacter
{
public:
	Hero();
	Hero(const Hero& other);
	~Hero();
	void operator=(const Hero& other);

	void handleEvent(const IGameState& gameState, sf::Event event);
	void update(IGameState& gameState);
	void draw(IGameState& gameState, sf::RenderWindow& window);

private:
	
	sf::Sprite m_sprite;
};

#endif

