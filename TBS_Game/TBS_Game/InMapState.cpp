#include "InMapState.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "GameUtilities.h"
#include "Json.h"

InMapState::InMapState(Game& game, std::string filename) :
	m_game(game),
	m_cursor("cursor") {	

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
	m_selectedx = 0;
	m_selectedy = 0;
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

	//Load the tiles into the grid
	Json::Value tileRoot = NULL;
	std::string terrainID;
	std::stringstream tileStream;
	bool traversable = true;
	for (unsigned int i = 0; i < m_tiles.size(); i++) {
		for (unsigned int j = 0; j < m_tiles[i].size(); j++) {
			tileStream.str("");
			tileStream << i << " " << j;
			tileRoot = root[tileStream.str()];
			if (tileRoot != NULL) {
				if (tileRoot.isMember("terrainID") && tileRoot["terrainID"].isString()) {
					terrainID = tileRoot["terrainID"].asString();
				}
				else {
					std::stringstream exitStream;
					exitStream << i << " " << j;
					GameUtilities::exitWithMessage("Failed to load terrainID for " + exitStream.str() + " from " + filename);
				}
				if (tileRoot.isMember("traversable") && tileRoot["traversable"].isBool()) {
					traversable = tileRoot["traversable"].asBool();
				}
				else {
					std::stringstream exitStream;
					exitStream << i << " " << j;
					GameUtilities::exitWithMessage("Failed to load traversable for " + exitStream.str() + " from " + filename);
				}
			}
			else {
				std::stringstream exitStream;
				exitStream << i << " " << j;
				GameUtilities::exitWithMessage("Failed to load " + exitStream.str() + " from " + filename);
			}

			m_tiles[i][j] = MapTile(terrainID, traversable, i, j, m_game.appInfo().tileSize());
		}
	}

	//initialize cursor to center of screen
	m_cursor.sprite().setPosition(game.appInfo().centerScreenx() - m_game.appInfo().tileSize() / 2, game.appInfo().centerScreeny() - m_game.appInfo().tileSize() / 2);

	//initialize music. Doesn't start playing yet
	m_music.openFromFile("../Assets/Sounds/level_1.wav");
}

InMapState::~InMapState() {
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
		if (event.type == sf::Event::EventType::KeyPressed) {
			if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
				moveSelected(m_selectedx-1, m_selectedy);
			}
			else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
				moveSelected(m_selectedx+1, m_selectedy);
			}
			else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
				moveSelected(m_selectedx, m_selectedy+1);
			}
			else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
				moveSelected(m_selectedx, m_selectedy-1);
			}
			else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space) {
				m_game.requestQuit();
			}

		}

		if (event.type == sf::Event::Closed) {
			m_game.requestQuit();
		}
	}
}

void InMapState::update() {
	m_cursor.update();
}

void InMapState::draw(){
	m_game.mainWindow()->clear(sf::Color::Black);

	//Draw tiles
	
	int drawx = 0;
	int	drawy = 0;
	for (unsigned int i = 0; i < m_tiles.size(); i++) {
		for (unsigned int j = 0; j < m_tiles[i].size(); j++) {
			drawx = (m_game.appInfo().centerScreenx() - m_game.appInfo().tileSize() / 2) + (i - m_selectedx) * m_game.appInfo().tileSize();
			drawy = (m_game.appInfo().centerScreeny() - m_game.appInfo().tileSize() / 2) + (j - m_selectedy) * m_game.appInfo().tileSize();
			m_tiles[i][j].draw(m_game, drawx , drawy);
		}
	}

	//Draw cursor overlay
	m_game.mainWindow()->draw(m_cursor.sprite());

	m_game.mainWindow()->display();
}

void InMapState::moveSelected(int x, int y) {
	if (x < 0 || y < 0 || x > m_tiles.size()-1 || y > m_tiles[x].size()-1) {
		return;
	}

	m_selectedx = x;
	m_selectedy = y;
}