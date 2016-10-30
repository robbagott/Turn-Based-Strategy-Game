#include "InMapState.h"
#include "TextureManager.h"
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
	m_selectedCharacter(NULL),
	m_turnChangeSpriteShowing(true) {

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

	//Initialize turnChangeSprite
	sf::Texture* turnTexture = TextureManager::get().load("../Assets/Graphics/player_turn.png");
	m_turnChangeSprite.setTexture(*turnTexture);
	m_turnChangeSpriteClock = new sf::Clock();
	m_turnChangeSpriteClock->restart();
}

void InMapState::cleanup() {
	m_music.stop();
}

void InMapState::pause() {}
void InMapState::resume() {}

void InMapState::handleEvents() {
	if (m_turnChangeSpriteShowing) {
		return;
	}

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
					m_menus.push_back(new CharacterSelectMenu(*characterAt(m_selected.x, m_selected.y), *this));
					m_cursor.setAnimation("white", true);
				}
			}
			break;
		case MS_CHARACTERPLACED: {
			CharacterSelectMenu::Option choice = m_menus.back()->handleEvent(event);

			if (choice == CharacterSelectMenu::Option::CSM_WAIT) {
				m_selectedCharacter->setMoved(true);

				delete m_menus.back();
				m_menus.pop_back();
				m_moveSpan.clear();
				m_attackSpan.clear();
				m_selectedCharacter = NULL;
				m_mapState = MS_DEFAULT;

				if(allCharactersMoved()) {
					m_turn = Turn::ENEMYTURN;
					changeTurn();
				}
			}
			if (choice == CharacterSelectMenu::Option::CSM_ATTACK) {
				//Check for characters to attack near character. Place cursor and switch state if so.
				if (characterAt(m_selected.x - 1, m_selected.y)) {
					moveSelected(m_selected.x - 1, m_selected.y);
					m_mapState = MS_CHARACTERATTACKING;
					delete m_menus.back();
					m_menus.pop_back();
				}
				else if (characterAt(m_selected.x + 1, m_selected.y)) {
					moveSelected(m_selected.x + 1, m_selected.y);
					m_mapState = MS_CHARACTERATTACKING;
					delete m_menus.back();
					m_menus.pop_back();
				}
				else if (characterAt(m_selected.x, m_selected.y + 1)) {
					moveSelected(m_selected.x, m_selected.y + 1);
					m_mapState = MS_CHARACTERATTACKING;
					delete m_menus.back();
					m_menus.pop_back();
				}
				else if (characterAt(m_selected.x, m_selected.y - 1)) {
					moveSelected(m_selected.x, m_selected.y - 1);
					m_mapState = MS_CHARACTERATTACKING;
					delete m_menus.back();
					m_menus.pop_back();
				}
			}
			break;
		}
		case MS_CHARACTERATTACKING:
			if (event.type == sf::Event::EventType::KeyPressed) {
				if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
					if (std::find(m_attackSpan.begin(), m_attackSpan.end(), sf::Vector2i(m_selectedCharacter->gridPos().x - 1, m_selectedCharacter->gridPos().y)) != m_attackSpan.end()
						&& characterAt(m_selectedCharacter->gridPos().x - 1, m_selectedCharacter->gridPos().y) != NULL) {
						moveSelected(m_selectedCharacter->gridPos().x - 1, m_selectedCharacter->gridPos().y);
					}
				}
				else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
					if (std::find(m_attackSpan.begin(), m_attackSpan.end(), sf::Vector2i(m_selectedCharacter->gridPos().x + 1, m_selectedCharacter->gridPos().y)) != m_attackSpan.end()
						&& characterAt(m_selectedCharacter->gridPos().x + 1, m_selectedCharacter->gridPos().y) != NULL) {
						moveSelected(m_selectedCharacter->gridPos().x + 1, m_selectedCharacter->gridPos().y);
					}
				}
				else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
					if(std::find(m_attackSpan.begin(), m_attackSpan.end(), sf::Vector2i(m_selectedCharacter->gridPos().x, m_selectedCharacter->gridPos().y + 1)) != m_attackSpan.end()
						&& characterAt(m_selectedCharacter->gridPos().x, m_selectedCharacter->gridPos().y + 1) != NULL) {
						moveSelected(m_selectedCharacter->gridPos().x, m_selectedCharacter->gridPos().y + 1);
					}
				}
				else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
					if (std::find(m_attackSpan.begin(), m_attackSpan.end(), sf::Vector2i(m_selectedCharacter->gridPos().x, m_selectedCharacter->gridPos().y - 1)) != m_attackSpan.end()
						&& characterAt(m_selectedCharacter->gridPos().x, m_selectedCharacter->gridPos().y - 1) != NULL) {
						moveSelected(m_selectedCharacter->gridPos().x, m_selectedCharacter->gridPos().y - 1);
					}
				}
				else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space) {
					ICharacter* attackedCharacter = characterAt(m_selected.x, m_selected.y);
					m_selectedCharacter->attack(attackedCharacter);
					if (!attackedCharacter->isAlive()) {
						delete attackedCharacter;
						m_characters.erase(std::find(m_characters.begin(), m_characters.end(), attackedCharacter));
					}

					//Change turn if appropriate
					m_selectedCharacter->setMoved(true);
					if (allCharactersMoved()) {
						changeTurn();
					}

					m_moveSpan.clear();
					m_attackSpan.clear();
					m_selectedCharacter = NULL;
					m_mapState = MS_DEFAULT;	
				}
				else if (event.key.code == sf::Keyboard::BackSpace) {
					m_menus.push_back(new CharacterSelectMenu(*characterAt(m_selectedCharacter->gridPos().x, m_selectedCharacter->gridPos().y), *this));
					m_selected = sf::Vector2i(m_selectedCharacter->gridPos().x, m_selectedCharacter->gridPos().y);
					m_mapState = MS_CHARACTERPLACED;
				}
			}
			break;
		}

		//At any time, quit if closed
		if (event.type == sf::Event::Closed) {
			m_game.requestQuit();
		}
	}
}

void InMapState::aiControl() {
	//Handle input
	sf::Event event;
	while (m_game.mainWindow()->pollEvent(event)) {

		//At any time, quit if closed
		if (event.type == sf::Event::Closed) {
			m_game.requestQuit();
		}

		changeTurn();
	}
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

	//Check for turnChangeTimer and update accordingly
	if (m_turnChangeSpriteClock != NULL) {
		if (m_turnChangeSpriteClock->getElapsedTime().asSeconds() > 1.0) {
			if (m_turn == Turn::ENEMYTURN) {
				TextureManager::get().free("../Assets/Graphics/enemy_turn.png");
			}
			else {
				TextureManager::get().free("../Assets/Graphics/player_turn.png");
			}
			m_turnChangeSpriteShowing = false;

			delete m_turnChangeSpriteClock;
			m_turnChangeSpriteClock = NULL;
		}
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

	//Draw a turn change graphic if present
	if (m_turnChangeSpriteShowing) {
		m_game.mainWindow()->draw(m_turnChangeSprite);
	}
	m_game.mainWindow()->display();
}

void InMapState::moveSelected(const unsigned int& x, const unsigned int& y) {
	if (x < 0 || y < 0 || x > m_tiles.size()-1 || y > m_tiles[x].size()-1) {
		return;
	}

	//Remove overlay of character at old selected tile
	ICharacter* character = characterAt(m_selected.x, m_selected.y);
	if (character != NULL) {
		character->showOverlay(false);
	}

	m_selected.x = x;
	m_selected.y = y;

	//Add the new character's overlay if there is a character on the tile
	character = characterAt(m_selected.x, m_selected.y);
	if (character != NULL) {
		character->showOverlay(true);
	}
	else if (m_selectedCharacter != NULL) {
		m_selectedCharacter->showOverlay(true);
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

bool InMapState::allCharactersMoved() {
	for (int i = 0; i < m_characters.size(); ++i) {
		if (m_characters[i]->isMoved() == false && m_characters[i]->friendly()) {
			return Turn::PLAYERTURN;
		}
	}
	return Turn::ENEMYTURN;
}

void InMapState::changeTurn() {
	if (m_turn == Turn::PLAYERTURN) {
		m_turn = Turn::ENEMYTURN;
	}
	else {
		m_turn = Turn::PLAYERTURN;
	}

	//set texture for m_turnChangeSprite
	if (m_turn == Turn::PLAYERTURN) {
		sf::Texture* texture = TextureManager::get().load("../Assets/Graphics/player_turn.png");
		m_turnChangeSprite.setTexture(*texture);

	}
	else {
		sf::Texture* texture = TextureManager::get().load("../Assets/Graphics/enemy_turn.png");
		m_turnChangeSprite.setTexture(*texture);
	}
	m_turnChangeSpriteShowing = true;
	m_turnChangeSpriteClock = new sf::Clock();
	m_turnChangeSpriteClock->restart();

	//TODO refresh character move states, sprite colors (like grey to blue), etc.
}