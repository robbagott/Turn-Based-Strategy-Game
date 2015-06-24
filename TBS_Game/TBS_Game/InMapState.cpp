#include "InMapState.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <queue>
#include "GameUtilities.h"
#include "Json.h"
#include "hero.h"

InMapState::InMapState(Game& game, std::string filename) :
	m_game(game),
	m_cursor("cursor"),
	m_mapState(MS_DEFAULT),
	m_turn(PLAYERTURN), 
	m_selectedCharacter(NULL) {	

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
	for (unsigned int i = 0; i < tileRoot.size(); ++i) {
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
		m_tiles[x][y] = MapTile(terrainID, traversable, x, y, AppInfo::get()->tileSize());
	}

	//Load the units into the map
	Json::Value unitRoot = root["units"];
	std::string unitType;
	bool friendly = false;
	for (unsigned int i = 0; i < unitRoot.size(); ++i) {
		if (!unitRoot[i].isMember("x") || !unitRoot[i]["x"].isInt()) {
			GameUtilities::exitWithMessage("Failed to load x for units from " + filename);

		}
		x = unitRoot[i]["x"].asInt();
		if (!unitRoot[i].isMember("y") || !unitRoot[i]["y"].isInt()) {
			GameUtilities::exitWithMessage("Failed to load x for units from " + filename);

		}
		y = unitRoot[i]["y"].asInt();
		if (!unitRoot[i].isMember("unit_type") || !unitRoot[i]["unit_type"].isString()) {
			GameUtilities::exitWithMessage("Failed to load unit_type for units from" + filename);

		}
		unitType = unitRoot[i]["unit_type"].asString();
		if (!unitRoot[i].isMember("friendly") || !unitRoot[i]["friendly"].isBool()) {
			GameUtilities::exitWithMessage("Failed to load friendly for units from " + filename);

		}
		friendly = unitRoot[i]["friendly"].asBool();
		
		m_characters.push_back(new Hero(*this, unitType, friendly));
		m_characters.back()->setGridPos(x, y);
	}

	//initialize cursor
	m_cursor.sprite().setPosition((float)(AppInfo::get()->centerScreenx() - AppInfo::get()->tileSize() / 2), (float)(AppInfo::get()->centerScreeny() - AppInfo::get()->tileSize() / 2));
	m_cursor.setAnimation("white", true);

	//Initialize focal point to center of map
	m_focalTile = sf::Vector2i(tilesWide/2, tilesHigh/2);

	//initialize music. Doesn't start playing yet
	m_music.openFromFile("../Assets/Sounds/level_1.wav");

	//initialize move_span sprite
	m_moveSpanSprite.setSpriteSheet("move_span");
	m_moveSpanSprite.setAnimation("move", true);
}

InMapState::~InMapState() {
	for (unsigned int i = 0; i < m_characters.size(); ++i) {
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
	if (m_turn == PLAYERTURN) {
		playerControl();
	}
	else if (m_turn == ENEMYTURN) {
		aiControl();
	}
}

void InMapState::playerControl() {
	sf::Event event;
	while (m_game.mainWindow()->pollEvent(event)) {

		switch (m_mapState) {
		case MS_DEFAULT:
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
			break;
		case MS_CHARACTERSELECTED:
			if (event.type == sf::Event::EventType::KeyPressed) {
				if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
					if (std::find(m_moveSpan.begin(), m_moveSpan.end(), sf::Vector2i(m_selected.x - 1, m_selected.y)) != m_moveSpan.end()) {
						moveSelected(m_selected.x - 1, m_selected.y);
					}
				}
				else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
					if (std::find(m_moveSpan.begin(), m_moveSpan.end(), sf::Vector2i(m_selected.x + 1, m_selected.y)) != m_moveSpan.end()) {
						moveSelected(m_selected.x + 1, m_selected.y);
					}
				}
				else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
					if (std::find(m_moveSpan.begin(), m_moveSpan.end(), sf::Vector2i(m_selected.x, m_selected.y + 1)) != m_moveSpan.end()) {
						moveSelected(m_selected.x, m_selected.y + 1);
					}
				}
				else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
					if (std::find(m_moveSpan.begin(), m_moveSpan.end(), sf::Vector2i(m_selected.x, m_selected.y - 1)) != m_moveSpan.end()) {
						moveSelected(m_selected.x, m_selected.y - 1);
					}
				}
				else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space) {
					m_selectedCharacter->setGridPos(m_selected.x, m_selected.y);
					m_mapState = MS_CHARACTERPLACED;
					m_menus.push_back(new CharacterSelectMenu());
					m_cursor.setAnimation("white", true);
					m_moveSpan.clear();
					m_attackSpan.clear();
				}
				else if (event.key.code == sf::Keyboard::Escape) {
					m_game.requestQuit();
				}
			}
			break;
		case MS_CHARACTERPLACED:
			if (event.type == sf::Event::EventType::KeyPressed) {

				m_menus.back()->handleEvent(event);

				if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space) {
					delete m_menus.back();
					m_menus.pop_back();

					m_selectedCharacter = NULL;
					m_mapState = MS_DEFAULT;
				}
			}
			break;
		}

		//At any time, quit if escape is pressed
		if (event.type == sf::Event::Closed) {
			m_game.requestQuit();
		}
	}
}

void InMapState::aiControl() {
	//Handle input
	sf::Event event;
	while (m_game.mainWindow()->pollEvent(event)) {

		//At any time, quit if escape is pressed
		if (event.type == sf::Event::Closed) {
			m_game.requestQuit();
		}
	}

	std::cerr << "enemy turn" << std::endl;
}

void InMapState::update() {
	m_cursor.update();

	for (unsigned int i = 0; i < m_characters.size(); i++) {
		m_characters[i]->update();
	}

	if (m_mapState == MS_CHARACTERPLACED) {
		for (unsigned int i = 0; i < m_menus.size(); ++i)
		m_menus[i]->update();
	}
}

void InMapState::draw() {
	m_game.mainWindow()->clear(sf::Color::Black);

	//Draw tiles
	int drawx = 0;
	int	drawy = 0;
	for (unsigned int i = 0; i < m_tiles.size(); i++) {
		for (unsigned int j = 0; j < m_tiles[i].size(); j++) {
			drawx = (AppInfo::get()->centerScreenx() - AppInfo::get()->tileSize() / 2) + (i - m_focalTile.x) * AppInfo::get()->tileSize();
			drawy = (AppInfo::get()->centerScreeny() - AppInfo::get()->tileSize() / 2) + (j - m_focalTile.y) * AppInfo::get()->tileSize();
			m_tiles[i][j].draw(m_game, drawx , drawy);
		}
	}

	//Draw moveSpan highlights
	m_moveSpanSprite.setAnimation("move", true);
	for (unsigned int i = 0; i < m_moveSpan.size(); ++i) {

		drawx = (AppInfo::get()->centerScreenx() - AppInfo::get()->tileSize() / 2) + (m_moveSpan[i].x - m_focalTile.x) * AppInfo::get()->tileSize();
		drawy = (AppInfo::get()->centerScreeny() - AppInfo::get()->tileSize() / 2) + (m_moveSpan[i].y - m_focalTile.y) * AppInfo::get()->tileSize();
		m_moveSpanSprite.sprite().setPosition((float)drawx, (float)drawy);
		m_game.mainWindow()->draw(m_moveSpanSprite.sprite());
	}

	m_moveSpanSprite.setAnimation("attack", true);
	for (unsigned int i = 0; i < m_attackSpan.size(); ++i) {
		drawx = (AppInfo::get()->centerScreenx() - AppInfo::get()->tileSize() / 2) + (m_attackSpan[i].x - m_focalTile.x) * AppInfo::get()->tileSize();
		drawy = (AppInfo::get()->centerScreeny() - AppInfo::get()->tileSize() / 2) + (m_attackSpan[i].y - m_focalTile.y) * AppInfo::get()->tileSize();
		m_moveSpanSprite.sprite().setPosition((float)drawx, (float)drawy);
		m_game.mainWindow()->draw(m_moveSpanSprite.sprite());
	}

	//Draw cursor overlay
	drawx = (AppInfo::get()->centerScreenx() - AppInfo::get()->tileSize() / 2) + (m_selected.x - m_focalTile.x) * AppInfo::get()->tileSize();
	drawy = (AppInfo::get()->centerScreeny() - AppInfo::get()->tileSize() / 2) + (m_selected.y - m_focalTile.y) * AppInfo::get()->tileSize();
	m_cursor.sprite().setPosition((float)drawx, (float)drawy);
	m_game.mainWindow()->draw(m_cursor.sprite());

	//Draw characters
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		sf::Vector2i gridPos = m_characters[i]->gridPos();
		drawx = (AppInfo::get()->centerScreenx() - AppInfo::get()->tileSize() / 2) + (gridPos.x - m_focalTile.x) * AppInfo::get()->tileSize();
		drawy = (AppInfo::get()->centerScreeny() - AppInfo::get()->tileSize() / 2) + (gridPos.y - m_focalTile.y) * AppInfo::get()->tileSize();
		m_characters[i]->setPosition(drawx, drawy);
		m_characters[i]->draw(*m_game.mainWindow());
	}

	//Draw any menus
	if (m_mapState == MS_CHARACTERPLACED) {
		for (unsigned int i = 0; i < m_menus.size(); ++i) {
			m_menus[i]->draw(*m_game.mainWindow());
		}
	}
	m_game.mainWindow()->display();
}

void InMapState::moveSelected(const unsigned int& x, const unsigned int& y) {
	if (x < 0 || y < 0 || x > m_tiles.size()-1 || y > m_tiles[x].size()-1) {
		return;
	}

	ICharacter* character = characterAt(m_selected.x, m_selected.y);
	if (character != NULL && m_selectedCharacter == NULL) {
		character->showOverlay(false);
	}

	m_selected.x = x;
	m_selected.y = y;

	character = characterAt(m_selected.x, m_selected.y);
	if (character != NULL && m_selectedCharacter == NULL) {
		character->showOverlay(true);
	}

	//Deal with going off screen. (Pull focal tile toward selected) 
}

void InMapState::onSelectPress() {

	ICharacter* character = characterAt(m_selected.x, m_selected.y);
	if (character != NULL && character->friendly() == true) {
		m_selectedCharacter = character;
		m_cursor.setAnimation("blue", true);
		populateMoveSpan(m_selectedCharacter->movePoints(), m_selected.x, m_selected.y);
		m_mapState = MS_CHARACTERSELECTED;
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

//Implemented as a modified BFS starting from x, and y
void InMapState::populateMoveSpan(int movePoints, const int& x, const int& y) {
	//stupid check
	if (movePoints < 0) { return; }
	
	struct Vec4 {
		int x, y, z, a;
	};

	//initialize attack range
	int attackPoints = 1;

	//Used in bfs to order traversals without recursion
	std::queue<Vec4> queue;
	queue.push({ x, y, movePoints, attackPoints});
	m_moveSpan.push_back(sf::Vector2i(x, y));
	Vec4 current = { 0, 0, 0 };
	while (!queue.empty()) {
		current = queue.front();
		queue.pop();

		if (current.z >= 0 || current.a >= 0)	{
			//Right
			if (current.x + 1 < m_tiles.size()) {
				if (std::find(m_moveSpan.begin(), m_moveSpan.end(), sf::Vector2i(current.x + 1, current.y)) == m_moveSpan.end()) {
					if (characterAt(current.x + 1, current.y) == NULL && current.z - m_tiles[current.x + 1][current.y].moveCost() >= 0) {
						queue.push({ current.x + 1, current.y, current.z - m_tiles[current.x + 1][current.y].moveCost(), current.a });
						m_moveSpan.push_back(sf::Vector2i(current.x + 1, current.y));
					}
					else if (std::find(m_attackSpan.begin(), m_attackSpan.end(), sf::Vector2i(current.x + 1, current.y)) == m_attackSpan.end()
							&& current.a - 1 >= 0) {
						queue.push({ current.x + 1, 0, current.a - 1 });
						m_attackSpan.push_back(sf::Vector2i(current.x + 1, current.y));
					}
				}
			}
			//left
			if (current.x - 1 >= 0) {
				if (std::find(m_moveSpan.begin(), m_moveSpan.end(), sf::Vector2i(current.x - 1, current.y)) == m_moveSpan.end()) {
					if (characterAt(current.x - 1, current.y) == NULL && current.z - m_tiles[current.x - 1][current.y].moveCost() >= 0) {
						queue.push({ current.x - 1, current.y, current.z - m_tiles[current.x - 1][current.y].moveCost(), current.a });
						m_moveSpan.push_back(sf::Vector2i(current.x - 1, current.y));
					}
					else if (std::find(m_attackSpan.begin(), m_attackSpan.end(), sf::Vector2i(current.x - 1, current.y)) == m_attackSpan.end()
						&& current.a - 1 >= 0) {
						queue.push({ current.x - 1, current.y, 0, current.a - 1 });
						m_attackSpan.push_back(sf::Vector2i(current.x - 1, current.y));
					}
				}
			}
			//up
			if (current.y - 1 >= 0) {
				if (std::find(m_moveSpan.begin(), m_moveSpan.end(), sf::Vector2i(current.x, current.y - 1)) == m_moveSpan.end()) {
					if (characterAt(current.x, current.y - 1) == NULL && current.z - m_tiles[current.x][current.y - 1].moveCost() >= 0) {
						queue.push({ current.x, current.y - 1, current.z - m_tiles[current.x][current.y - 1].moveCost(), current.a });
						m_moveSpan.push_back(sf::Vector2i(current.x, current.y - 1));
					}
					else if (std::find(m_attackSpan.begin(), m_attackSpan.end(), sf::Vector2i(current.x, current.y - 1)) == m_attackSpan.end()
						&& current.a - 1 >= 0) {
						queue.push({ current.x, current.y - 1, 0, current.a - 1 });
						m_attackSpan.push_back(sf::Vector2i(current.x, current.y - 1));
					}
				}
			}
			//down
			if (current.y + 1 < m_tiles[current.x].size()) {
				if (std::find(m_moveSpan.begin(), m_moveSpan.end(), sf::Vector2i(current.x, current.y + 1)) == m_moveSpan.end()) {
					if (characterAt(current.x, current.y + 1) == NULL && current.z - m_tiles[current.x][current.y + 1].moveCost() >= 0) {
						queue.push({ current.x, current.y + 1, current.z - m_tiles[current.x][current.y + 1].moveCost(), current.a });
						m_moveSpan.push_back(sf::Vector2i(current.x, current.y + 1));
					}
					else if (std::find(m_attackSpan.begin(), m_attackSpan.end(), sf::Vector2i(current.x, current.y + 1)) == m_attackSpan.end()
						&& current.a - 1 >= 0) {
						queue.push({ current.x, current.y + 1, 0, current.a - 1 });
						m_attackSpan.push_back(sf::Vector2i(current.x, current.y + 1));
					}
				}
			}
		}
	}
}