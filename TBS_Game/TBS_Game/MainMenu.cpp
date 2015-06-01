#include "MainMenu.h"
#include "NullGameState.h"
#include <iostream>

MainMenu::MainMenu() {
	if (!m_texture.loadFromFile("../Assets/Graphics/MainMenu.bmp")) {
		std::cerr << "No Main Menu texture loaded." << std::endl;
		exit(1);
	}
	m_sprite.setTexture(m_texture);
}

MainMenu::~MainMenu() {}

void MainMenu::init() {
	std::cout << "Entering main menu state" << std::endl;
}
void MainMenu::cleanup() {}
void MainMenu::pause() {}
void MainMenu::resume() {}

void MainMenu::handleEvents(IStateBasedGame& game) {

}
void MainMenu::update(IStateBasedGame& game) {

	sf::Event currentEvent;
	while (game.mainWindow()->pollEvent(currentEvent)) {
		if (currentEvent.type == sf::Event::EventType::KeyPressed) {
			IGameState* newState = new NullGameState();
			game.requestChangeState(*newState);
		}
		else if (currentEvent.type == sf::Event::EventType::Closed) {
			game.requestQuit();
		}
	}
}
void MainMenu::draw(IStateBasedGame& game) {
	game.mainWindow()->clear(sf::Color::Black);
	game.mainWindow()->draw(m_sprite);
	game.mainWindow()->display();
}