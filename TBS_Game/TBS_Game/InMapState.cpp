#include "InMapState.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "GameUtilities.h"
#include "Json.h"

InMapState::InMapState(Game& game, std::string filename) {	
	std::ifstream mapStream(filename, std::ios_base::binary);
	if (!mapStream.good()) {
		GameUtilities::exitWithMessage("failed to load " + filename + " in InMapState constructor");
	}
	Json::Value root = NULL;
	mapStream >> root;

	//Collect basic map information from map json file
	int tilesWide = 0;
	if (root.get("width", "").isInt()) {
		tilesWide = root.get("width", "").asInt();
	}
	else {
		GameUtilities::exitWithMessage("Failed to load width from " + filename);
	}

	int tilesHigh = 0;
	if (root.get("height", "").isInt()) {
		tilesHigh = root.get("height", "").asInt();
	}
	else {
		GameUtilities::exitWithMessage("Failed to load height from " + filename);
	}

	//Resize the grid according to the configuration
	m_tiles.resize(tilesWide);
	for (unsigned int i = 0; i < m_tiles.size(); i++) {
		m_tiles[i].resize(tilesHigh);
	}
	m_selectedx = 0;
	m_selectedy = 0;
	int selectedx;
	if (root.get("startx", "").isInt()) {
		selectedx = root.get("startx", "").asInt();
	}
	else {
		GameUtilities::exitWithMessage("Failed to load startx from " + filename);
	}

	int selectedy;
	if (root.get("starty", "").isInt()) {
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
				if (tileRoot.get("terrainID", "").isString()) {
					terrainID = tileRoot.get("terrainID", "").asString();
				}
				else {
					std::stringstream exitStream;
					exitStream << i << " " << j;
					GameUtilities::exitWithMessage("Failed to load terrainID for " + exitStream.str() + " from " + filename);
				}
				if (tileRoot.get("traversable", "").isBool()) {
					traversable = tileRoot.get("traversable", "").asBool();
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

			m_tiles[i][j] = MapTile(terrainID, traversable, i, j);
		}
	}

	if (!m_cursorTexture.loadFromFile("../Assets/Graphics/cursor_overlay.png")) {
		GameUtilities::exitWithMessage("No cursor overlay texture loaded in InMapState constructor.");
	}
	m_cursorOverlay.setTexture(m_cursorTexture);
	m_cursorOverlay.setColor(sf::Color(255, 255, 255, 128));
	m_cursorOverlay.setPosition(game.appInfo().centerScreenx() - 8, game.appInfo().centerScreeny() - 8);

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

void InMapState::handleEvents(Game& game) {
	sf::Event event;
	while (game.mainWindow()->pollEvent(event)) {
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
				game.requestQuit();
			}

		}

		if (event.type == sf::Event::Closed) {
			game.requestQuit();
		}
	}
}

void InMapState::update(Game& game) {}

void InMapState::draw(Game& game){
	game.mainWindow()->clear(sf::Color::Black);

	//Draw tiles
	
	int drawx = 0;
	int	drawy = 0;
	for (unsigned int i = 0; i < m_tiles.size(); i++) {
		for (unsigned int j = 0; j < m_tiles[i].size(); j++) {
			drawx = (game.appInfo().centerScreenx() - 8) + (i - m_selectedx) * 16;
			drawy = (game.appInfo().centerScreeny() - 8) + (j - m_selectedy) * 16;
			m_tiles[i][j].draw(game, drawx , drawy);
		}
	}

	//Drar cursor overlay
	game.mainWindow()->draw(m_cursorOverlay);

	game.mainWindow()->display();
}

void InMapState::moveSelected(int x, int y) {
	if (x < 0 || y < 0 || x > m_tiles.size()-1 || y > m_tiles[x].size()-1) {
		return;
	}

	m_selectedx = x;
	m_selectedy = y;
}