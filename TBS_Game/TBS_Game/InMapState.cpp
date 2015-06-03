#include "InMapState.h"

InMapState::InMapState(std::string filename) {
	tiles.push_back( *(new std::vector<MapTile>()) );
	tiles[0].push_back( *(new MapTile("grass_1", true, 0, 0)) );
}

InMapState::~InMapState() {

}

void InMapState::init() {}

void InMapState::cleanup() {}

void InMapState::pause() {}
void InMapState::resume() {}

void InMapState::handleEvents(IStateBasedGame& game) {}
void InMapState::update(IStateBasedGame& game) {}
void InMapState::draw(IStateBasedGame& game){
	tiles[0][0].draw(game);
}