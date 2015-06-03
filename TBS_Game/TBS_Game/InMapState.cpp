#include "InMapState.h"

InMapState::InMapState(std::string filename) {
	tiles.push_back( *(new std::vector<MapTile>()) );
	tiles[0].push_back( *(new MapTile("forest_1", true, 0, 0)) );
}

InMapState::~InMapState() {

}

void InMapState::init() {}

void InMapState::cleanup() {}

void InMapState::pause() {}
void InMapState::resume() {}

void InMapState::handleEvents(IStateBasedGame& game) {
	sf::Event event;
	while (game.mainWindow()->pollEvent(event)) {

		if (event.type == sf::Event::Closed) {
			game.requestQuit();
		}
	}
}
void InMapState::update(IStateBasedGame& game) {}
void InMapState::draw(IStateBasedGame& game){
	game.mainWindow()->clear(sf::Color::Black);
	tiles[0][0].draw(game);
	game.mainWindow()->display();
}