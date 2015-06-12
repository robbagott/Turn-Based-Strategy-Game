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

	sf::Vector2f getPosition() const;
	void setPosition(const sf::Vector2f&);

	sf::Vector2u getGridPos();
	void setGridPos(sf::Vector2u gridPos);

private:
	
	SpriteSheet m_spriteSheet;
	std::string m_name;

	int m_gridx, m_gridy;
};

#endif
