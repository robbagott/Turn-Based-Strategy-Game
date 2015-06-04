#include "MapTile.h"
#include <fstream>
#include "GameUtilities.h"
#include "json.h"

MapTile::MapTile(std::string terrainID, bool traversable, int posx, int posy) : 
	m_traversable(traversable), 
	m_gridPosx(posx), 
	m_gridPosy(posy), 
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

MapTile::MapTile() :
	m_traversable (false),
	m_gridPosx(0),
	m_gridPosy(0),
	m_occupied(false),
	m_ground(false),
	m_deployable(false),
	m_moveCost(0),
	m_evasionBoost(0) 
{}
MapTile::~MapTile() {}

int MapTile::moveCost() {
	return m_moveCost;
}
bool MapTile::occupied() {
	return m_occupied;
}
bool MapTile::traversable() {
	return m_traversable;
}
bool MapTile::ground() {
	return m_ground;
}

void MapTile::draw(IStateBasedGame& game, int xpos, int ypos) {
	m_sprite.setPosition(xpos, ypos);
	game.mainWindow()->draw(m_sprite);
}