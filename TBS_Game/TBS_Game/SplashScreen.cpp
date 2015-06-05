#include "SplashScreen.h"
#include "MainMenu.h"
#include <iostream>

SplashScreen::SplashScreen(std::string filename) {
	if (!m_texture.loadFromFile(filename)) {
		std::cerr << "No splash screen texture loaded." << std::endl;
		char x;
		std::cin >> x;
		exit(1);
	}
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

void SplashScreen::handleEvents(Game& game) {
	static sf::Time startTime = m_clock.getElapsedTime();

	sf::Event currentEvent;
	while (game.mainWindow()->pollEvent(currentEvent)) {
		if (currentEvent.type == sf::Event::EventType::KeyPressed) {
			IGameState* newState = new MainMenu();
			game.requestChangeState(*newState);
		}
		if (currentEvent.type == sf::Event::EventType::Closed) {
			game.requestQuit();
		}
	}

	if ((m_clock.getElapsedTime() - startTime).asSeconds() > 3) {
		IGameState* newState = new MainMenu();
		game.requestChangeState(*newState);
	}
}

void SplashScreen::update(Game& game) {
	
}

void SplashScreen::draw(Game& game) {
	game.mainWindow()->clear(sf::Color::Black);
	game.mainWindow()->draw(m_sprite);
	game.mainWindow()->display();
}