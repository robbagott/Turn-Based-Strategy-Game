#include "MainMenu.h"
#include "NullGameState.h"
#include <cassert>

MainMenu::MainMenu() {
	assert(m_texture.loadFromFile("../Assets/Graphics/MainMenu.bmp") == true);
	m_sprite.setTexture(m_texture);
}

MainMenu::~MainMenu() {}

void MainMenu::init() {}
void MainMenu::cleanup() {}
void MainMenu::pause() {}
void MainMenu::resume() {}

void MainMenu::handleEvents(IStateBasedGame& game) {

}
void MainMenu::update(IStateBasedGame& game) {
	static sf::Time startTime = m_clock.getElapsedTime();

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

	if ((m_clock.getElapsedTime() - startTime).asSeconds() > 5) {
		IGameState* newState = new NullGameState();
		game.requestChangeState(*newState);
	}
}
void MainMenu::draw(IStateBasedGame& game) {
	game.mainWindow()->clear(sf::Color::Black);
	game.mainWindow()->draw(m_sprite);
	game.mainWindow()->display();
}