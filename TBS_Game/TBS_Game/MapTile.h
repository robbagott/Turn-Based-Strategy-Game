#ifndef MAPTILE_H
#define MAPTILE_H

#include <SFML/Graphics.hpp>
#include "Game.h"
#include "SpriteSheet.h"

class MapTile
{
public:
	MapTile(); 
	MapTile(std::string terrainID, bool traversable, int posx, int posy, int tileSize);
	MapTile(const MapTile& otherTile);
	~MapTile(); 
	void operator=(const MapTile& otherTile);

	int moveCost() const;
	int evasionBoost() const;
	bool occupied() const;
	bool traversable() const;
	bool deployable() const;
	bool ground() const;
	const sf::Vector2i& gridPos() const;

	void draw(Game& game, const int& xpos, const int& ypos);
	
private:

	SpriteSheet m_spriteSheet;

	bool m_traversable;
	bool m_occupied;

	bool m_ground;
	bool m_deployable;
	unsigned int m_moveCost;
	unsigned int m_evasionBoost;

	sf::Vector2i m_gridPos;
};

#endif