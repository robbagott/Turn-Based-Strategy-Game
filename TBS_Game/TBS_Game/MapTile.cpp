#include "MapTile.h"
#include <fstream>
#include <iostream>
#include "GameUtilities.h"
#include "json.h"

MapTile::MapTile(std::string terrainID, bool traversable, int posx, int posy, int tileSize) :
	m_traversable(traversable), 
	m_gridPos(sf::Vector2i(posx, posy)), 
	m_occupied(false) {

	std::ifstream terrainStream("../Assets/Data/terrain.json", std::ios_base::binary);
	if (!terrainStream.good()) {
		GameUtilities::exitWithMessage("failed to load terrainStream in MapTile constructor");
	}
	Json::Value root = NULL;
	terrainStream >> root;
	Json::Value terrain = root[terrainID];

	if (terrain.isMember("isGround") && terrain["isGround"].isBool()) {
		m_ground = terrain["isGround"].asBool();
	}
	else {
		GameUtilities::exitWithMessage("Failed to load isGround from terrain.json with terrainID: " + terrainID);
	}

	if (terrain.isMember("moveCost") && terrain["moveCost"].isInt()) {
		m_moveCost = terrain["moveCost"].asInt();
	}
	else {
		GameUtilities::exitWithMessage("Failed to load moveCost from terrain.json with terrainID: " + terrainID);
	}

	if (terrain.isMember("evasionBoost") && terrain["evasionBoost"].isInt()) {
		m_evasionBoost = terrain["evasionBoost"].asInt();
	}
	else {
		GameUtilities::exitWithMessage("Failed to load evasionBoost from terrain.json with terrainID: " + terrainID);
	}

	if (terrain.isMember("deployable") && terrain["deployable"].isBool()) {
		m_deployable = terrain["deployable"].asBool();
	}
	else {
		GameUtilities::exitWithMessage("Failed to load deployable from terrain.json with terrainID: " + terrainID);
	}

	if (terrain.isMember("sprite_sheet") && terrain["sprite_sheet"].isString()) {
		m_spriteSheet.setSpriteSheet(terrain["sprite_sheet"].asString());
	}
	else {
		GameUtilities::exitWithMessage("Failed to load sprite_sheet from terrain.json with terrainID: " + terrainID);
	}
}

MapTile::MapTile(const MapTile& other) {
	*this = other;
}

MapTile::MapTile() :
	m_traversable (false),
	m_gridPos(sf::Vector2i(0,0)),
	m_occupied(false),
	m_ground(false),
	m_deployable(false),
	m_moveCost(0),
	m_evasionBoost(0) 
{}

MapTile::~MapTile() {}

void MapTile::operator=(const MapTile& otherTile) {
	m_spriteSheet = otherTile.m_spriteSheet;

	m_traversable = otherTile.m_traversable;
	m_occupied = otherTile.m_occupied;

	m_ground = otherTile.m_ground;
	m_deployable = otherTile.m_deployable;
	m_moveCost = otherTile.m_moveCost;
	m_evasionBoost = otherTile.m_evasionBoost;

	m_gridPos.x = otherTile.m_gridPos.x;
	m_gridPos.y = otherTile.m_gridPos.y;
}

void MapTile::draw(Game& game, const int& xpos, const int& ypos) {
	m_spriteSheet.sprite().setPosition((float)xpos, (float)ypos);
	game.mainWindow()->draw(m_spriteSheet.sprite());
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

const sf::Vector2i& MapTile::gridPos() const {
	return m_gridPos;
}

bool MapTile::deployable() const {
	return m_deployable;
}

int MapTile::evasionBoost() const {
	return m_evasionBoost;
}
