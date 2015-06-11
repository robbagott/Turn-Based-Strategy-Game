#ifndef MAPTILE_H
#define MAPTILE_H

#include <SFML/Graphics.hpp>
#include "Game.h"
#include "SpriteSheet.h"

class MapTile
{
public:
	enum TerrainID { TID_GRASS_1, TID_FOREST_1, TID_MOUNTAINS_1, TID_TID_ROAD_1 };
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
	int gridx() const;
	int gridy() const;

	void draw(Game& game, int xpos, int ypos);
	
private:

	SpriteSheet m_spriteSheet;

	bool m_traversable;
	bool m_occupied;

	bool m_ground;
	bool m_deployable;
	unsigned int m_moveCost;
	unsigned int m_evasionBoost;

	int m_gridx, m_gridy;
};

#endif