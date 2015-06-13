#include "InMapState.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "GameUtilities.h"
#include "Json.h"
#include "hero.h"

InMapState::InMapState(Game& game, std::string filename) :
	m_game(game),
	m_cursor("cursor"),
	m_characterIsSelected(false) {	

	std::ifstream mapStream(filename, std::ios_base::binary);
	if (!mapStream.good()) {
		GameUtilities::exitWithMessage("failed to load " + filename + " in InMapState constructor");
	}
	Json::Value root = NULL;
	mapStream >> root;

	//Collect basic map information from map json file
	int tilesWide = 0;
	int tilesHigh = 0;
	if (root.isMember("width") && root["width"].isInt()) {
		tilesWide = root["width"].asInt();
	}
	else {
		GameUtilities::exitWithMessage("Failed to load width from " + filename);
	}
	if (root.isMember("height") && root["height"].isInt()) {
		tilesHigh = root["height"].asInt();
	}
	else {
		GameUtilities::exitWithMessage("Failed to load height from " + filename);
	}

	//Resize the grid according to the configuration
	m_tiles.resize(tilesWide);
	for (unsigned int i = 0; i < m_tiles.size(); i++) {
		m_tiles[i].resize(tilesHigh);
	}

	//Get starting selected square. Use temp selected squares used so we can 
	//error check with call to moveSelected(); Not an error
	m_selected.x = 0;
	m_selected.y = 0;
	int selectedx = 0;
	int selectedy = 0;
	if (root.isMember("startx") && root["startx"].isInt()) {
		selectedx = root["startx"].asInt();
	}
	else {
		GameUtilities::exitWithMessage("Failed to load startx from " + filename);
	}

	if (root.isMember("starty") && root["starty"].isInt()) {
		selectedy = root.get("starty", "").asInt();
	}
	else {
		GameUtilities::exitWithMessage("Failed to load starty from " + filename);
	}

	moveSelected(selectedx, selectedy);
	if (!root.isMember("tiles")) {
		GameUtilities::exitWithMessage("failed to find tiles in level with filename " + filename);
	}
	//Load the tiles into the grid
	Json::Value tileRoot = root["tiles"];
	int x = 0, y = 0;
	std::string terrainID;
	bool traversable = true;
	for (int i = 0; i < (tilesHigh)*(tilesWide); ++i) {
		if (tileRoot[i] != NULL) {
			if (!tileRoot[i].isMember("x") || !tileRoot[i]["x"].isInt()) {
				GameUtilities::exitWithMessage("Failed to load x for tiles from " + filename);

			}
			x = tileRoot[i]["x"].asInt();
			if (!tileRoot[i].isMember("y") || !tileRoot[i]["y"].isInt()) {
				GameUtilities::exitWithMessage("Failed to load x for tiles from " + filename);

			}
			y = tileRoot[i]["y"].asInt();
			if (!tileRoot[i].isMember("terrainID") || !tileRoot[i]["terrainID"].isString()) {
				GameUtilities::exitWithMessage("Failed to load terrainID for tiles from " + filename);

			}
			terrainID = tileRoot[i]["terrainID"].asString();
			if (!tileRoot[i].isMember("traversable") || !tileRoot[i]["traversable"].isBool()) {
				GameUtilities::exitWithMessage("Failed to load traversable for tiles from " + filename);

			}
			traversable = tileRoot[i]["traversable"].asBool();
		}
		else {
			GameUtilities::exitWithMessage("Failed to load tiles from " + filename);
		}

		m_tiles[x][y] = MapTile(terrainID, traversable, x, y, m_game.appInfo().tileSize());
	}

	//initialize cursor
	m_cursor.sprite().setPosition((float)(game.appInfo().centerScreenx() - m_game.appInfo().tileSize() / 2), (float)(game.appInfo().centerScreeny() - m_game.appInfo().tileSize() / 2));
	m_cursor.setAnimation("white", true);
	//Initialize focal point to center of map
	m_focalTile = sf::Vector2i(tilesWide/2-1, tilesHigh/2-1);

	//initialize music. Doesn't start playing yet
	m_music.openFromFile("../Assets/Sounds/level_1.wav");

	//*****************TEMPORARY*****************
	ICharacter* hero = new Hero("x19");
	hero->setPosition( game.appInfo().centerScreenx() - m_game.appInfo().tileSize() / 2, game.appInfo().centerScreeny() - m_game.appInfo().tileSize() / 2);
	m_characters.push_back(hero);
}

InMapState::~InMapState() {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		delete m_characters[i];
	}
}

void InMapState::init() {
	m_music.play();
}

void InMapState::cleanup() {
	m_music.stop();
}

void InMapState::pause() {}
void InMapState::resume() {}

void InMapState::handleEvents() {
	sf::Event event;
	while (m_game.mainWindow()->pollEvent(event)) {

		if (m_characterIsSelected) {
			if (event.type == sf::Event::EventType::KeyPressed) {
				if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
					moveSelected(m_selected.x - 1, m_selected.y);
				}
				else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
					moveSelected(m_selected.x + 1, m_selected.y);
				}
				else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
					moveSelected(m_selected.x, m_selected.y + 1);
				}
				else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
					moveSelected(m_selected.x, m_selected.y - 1);
				}
				else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space) {
					m_selectedCharacter->setGridPos(m_selected.x, m_selected.y);
					m_characterIsSelected = false;
					m_cursor.setAnimation("white", true);
				}
				else if (event.key.code == sf::Keyboard::Escape) {
					m_game.requestQuit();
				}
			}
		}

		else {
			if (event.type == sf::Event::EventType::KeyPressed) {
				if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
					moveSelected(m_selected.x - 1, m_selected.y);
				}
				else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
					moveSelected(m_selected.x + 1, m_selected.y);
				}
				else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
					moveSelected(m_selected.x, m_selected.y + 1);
				}
				else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
					moveSelected(m_selected.x, m_selected.y - 1);
				}
				else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space) {
					onSelectPress();
				}
				else if (event.key.code == sf::Keyboard::Escape) {
					m_game.requestQuit();
				}
			}
		}

		if (event.type == sf::Event::Closed) {
			m_game.requestQuit();
		}
	}
}

void InMapState::update() {
	m_cursor.update();

	for (unsigned int i = 0; i < m_characters.size(); i++) {
		m_characters[i]->update(*this);
	}
}

void InMapState::draw() {
	m_game.mainWindow()->clear(sf::Color::Black);

	//Draw tiles
	int drawx = 0;
	int	drawy = 0;
	for (unsigned int i = 0; i < m_tiles.size(); i++) {
		for (unsigned int j = 0; j < m_tiles[i].size(); j++) {
			drawx = (m_game.appInfo().centerScreenx() - m_game.appInfo().tileSize() / 2) + (i - m_focalTile.x) * m_game.appInfo().tileSize();
			drawy = (m_game.appInfo().centerScreeny() - m_game.appInfo().tileSize() / 2) + (j - m_focalTile.y) * m_game.appInfo().tileSize();
			m_tiles[i][j].draw(m_game, drawx , drawy);
		}
	}

	//Draw cursor overlay
	drawx = (m_game.appInfo().centerScreenx() - m_game.appInfo().tileSize() / 2) + (m_selected.x - m_focalTile.x) * m_game.appInfo().tileSize();
	drawy = (m_game.appInfo().centerScreeny() - m_game.appInfo().tileSize() / 2) + (m_selected.y - m_focalTile.y) * m_game.appInfo().tileSize();
	m_cursor.sprite().setPosition((float)drawx, (float)drawy);
	m_game.mainWindow()->draw(m_cursor.sprite());

	//Draw characters
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		sf::Vector2i gridPos = m_characters[i]->gridPos();
		drawx = (m_game.appInfo().centerScreenx() - m_game.appInfo().tileSize() / 2) + (gridPos.x - m_focalTile.x) * m_game.appInfo().tileSize();
		drawy = (m_game.appInfo().centerScreeny() - m_game.appInfo().tileSize() / 2) + (gridPos.y - m_focalTile.y) * m_game.appInfo().tileSize();
		m_characters[i]->setPosition(drawx, drawy);
		m_characters[i]->draw(*this, *m_game.mainWindow());
	}

	m_game.mainWindow()->display();
}

void InMapState::moveSelected(const unsigned int& x, const unsigned int& y) {
	if (x < 0 || y < 0 || x > m_tiles.size()-1 || y > m_tiles[x].size()-1) {
		return;
	}

	m_selected.x = x;
	m_selected.y = y;

	//Deal with going off screen. (Pull focal tile toward selected) 
}

void InMapState::onSelectPress() {

	if ( (m_selectedCharacter = characterAt(m_selected.x, m_selected.y)) != NULL) {
		m_characterIsSelected = true;
		m_cursor.setAnimation("blue", true);
	}
	//Maybe play sound
	//Maybe choose what menu to display based on what's on selected tile
	//Maybe change cursor color
	//Maybe display places that hero can move
}

ICharacter* InMapState::characterAt(const int& x, const int& y) {
	sf::Vector2i pos(0, 0);
	for (unsigned int i = 0; i < m_characters.size(); ++i) {
		pos = m_characters[i]->gridPos();
		if (pos.x == x && pos.y == y) {
			return m_characters[i];
		}
	}

	return NULL;
}