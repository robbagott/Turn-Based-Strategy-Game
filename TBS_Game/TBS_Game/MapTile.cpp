#include "MapTile.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "GameUtilities.h"
#include "json.h"

MapTile::MapTile(std::string terrainID, bool traversable, int posx, int posy) : m_traversable(traversable), m_posx(posx), m_posy(posy) {
	std::ifstream terrainStream("../Assets/Data/terrain.json", std::ios_base::binary);
	Json::Value root;
	terrainStream >> root;

	Json::Value terrain = root[terrainID];
	if (terrain == NULL) {
		GameUtilities::exitWithMessage("Failed to load terrain from terrain.json with terrainID: " + terrainID);
	}

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
	m_sprite.setPosition(50, 50);
}
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

void MapTile::draw(IStateBasedGame& game) {
	std::cout << "HERE!: After draw" << std::endl;
	game.mainWindow()->draw(m_sprite);
	std::cout << "HERE!: After draw" << std::endl;
}