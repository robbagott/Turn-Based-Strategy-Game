#include "SplashScreen.h"
#include "MainMenu.h"
#include <cassert>

SplashScreen::SplashScreen(std::string filename) {
	assert(m_texture.loadFromFile(filename) == true);
	m_sprite.setTexture(m_texture);
}

SplashScreen::~SplashScreen() {}

void SplashScreen::init() {
	return;
}
void SplashScreen::cleanup() {
	return;
}

void SplashScreen::pause() {
	return;
}

void SplashScreen::resume() {
	return;
}

void SplashScreen::handleEvents(IStateBasedGame& game) {
	static sf::Time startTime = m_clock.getElapsedTime();

	sf::Event currentEvent;
	while (game.mainWindow()->pollEvent(currentEvent)) {
		if (currentEvent.type == sf::Event::EventType::KeyPressed ||
			currentEvent.type == sf::Event::EventType::Closed) {
			IGameState* newState = new MainMenu();
			game.requestChangeState(*newState);
		}
	}

	if ((m_clock.getElapsedTime() - startTime).asSeconds() > 5) {
		IGameState* newState = new MainMenu();
		game.requestChangeState(*newState);
	}
}

void SplashScreen::update(IStateBasedGame& game) {
	
}

void SplashScreen::draw(IStateBasedGame& game) {
	game.mainWindow()->clear(sf::Color::Black);
	game.mainWindow()->draw(m_sprite);
	game.mainWindow()->display();
}