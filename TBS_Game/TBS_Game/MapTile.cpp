#include "MapTile.h"
#include <fstream>
#include <iostream>
#include "GameUtilities.h"
#include "json.h"

MapTile::MapTile(std::string terrainID, bool traversable, int posx, int posy) : 
	m_traversable(traversable), 
	m_gridx(posx), 
	m_gridy(posy), 
	m_occupied(false) {

	std::ifstream terrainStream("../Assets/Data/terrain.json", std::ios_base::binary);
	if (!terrainStream.good()) {
		GameUtilities::exitWithMessage("failed to load terrainStream in MapTile constructor");
	}
	Json::Value root = NULL;
	terrainStream >> root;
	if (root == NULL) {
		GameUtilities::exitWithMessage("Failed to load root from terrain.json with terrainID: " + terrainID);
	}

	Json::Value terrain = root[terrainID];

	if (terrain.get("isGround", "").isBool()) {
		m_ground = terrain.get("isGround", "").asBool();
	}
	else {
		GameUtilities::exitWithMessage("Failed to load isGround from terrain.json with terrainID: " + terrainID);
	}

	if (terrain.get("moveCost", "").isInt()) {
		m_moveCost = terrain.get("moveCost", "").asInt();
	}
	else {
		GameUtilities::exitWithMessage("Failed to load moveCost from terrain.json with terrainID: " + terrainID);
	}

	if (terrain.get("evasionBoost", "").isInt()) {
		m_evasionBoost = terrain.get("evasionBoost", "").asInt();
	}
	else {
		GameUtilities::exitWithMessage("Failed to load evasionBoost from terrain.json with terrainID: " + terrainID);
	}

	if (terrain.get("deployable", "").isBool()) {
		m_deployable = terrain.get("deployable", "").asBool();
	}
	else {
		GameUtilities::exitWithMessage("Failed to load deployable from terrain.json with terrainID: " + terrainID);
	}

	if (terrain.get("texture", "").isString()) {
		if (!m_texture.loadFromFile( terrain.get("texture", "").asString()) ) {
			GameUtilities::exitWithMessage("Failed to load tile texture in " + terrainID);
		}
	}
	else {
		GameUtilities::exitWithMessage("Failed to load tile texture from terrain.json with terrainID: " + terrainID);
	}
	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
}
MapTile::MapTile(const MapTile& otherTile) {
	*this = otherTile;
}
MapTile::MapTile() :
	m_traversable (false),
	m_gridx(0),
	m_gridy(0),
	m_occupied(false),
	m_ground(false),
	m_deployable(false),
	m_moveCost(0),
	m_evasionBoost(0) 
{}
MapTile::~MapTile() {}
void MapTile::operator=(const MapTile& otherTile) {
	m_texture = otherTile.m_texture;
	m_sprite = otherTile.m_sprite;
	m_sprite.setTexture(m_texture);

	m_traversable = otherTile.m_traversable;
	m_occupied = otherTile.m_occupied;

	m_ground = otherTile.m_ground;
	m_deployable = otherTile.m_deployable;
	m_moveCost = otherTile.m_moveCost;
	m_evasionBoost = otherTile.m_evasionBoost;

	m_gridx = otherTile.m_gridx;
	m_gridy = otherTile.m_gridy;
}
void MapTile::draw(IStateBasedGame& game, int xpos, int ypos) {
	m_sprite.setPosition(xpos, ypos);
	game.mainWindow()->draw(m_sprite);
}
int MapTile::moveCost() const {
	return m_moveCost;
}
bool MapTile::occupied() const {
	return m_occupied;
}
bool MapTile::traversable() const {
	return m_traversable;
}
bool MapTile::ground() const {
	return m_ground;
}
int MapTile::gridx() const {
	return m_gridx;
}
int MapTile::gridy() const {
	return m_gridy;
}
bool MapTile::deployable() const {
	return m_deployable;
}
int MapTile::evasionBoost() const {
	return m_evasionBoost;
}
