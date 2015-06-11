#include "SplashScreen.h"
#include "MainMenu.h"
#include <iostream>

SplashScreen::SplashScreen(Game& game, std::string filename) : m_game(game) {
	m_texture = TextureManager::get().load("../Assets/Graphics/splash_screen.png");
	
	m_sprite.setTexture(*m_texture);
}

SplashScreen::~SplashScreen() {
	TextureManager::get().free("../Assets/Graphics/splash_screen.png");
}

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

void SplashScreen::handleEvents() {
	static sf::Time startTime = m_clock.getElapsedTime();

	sf::Event currentEvent;
	while (m_game.mainWindow()->pollEvent(currentEvent)) {
		if (currentEvent.type == sf::Event::EventType::KeyPressed) {
			IGameState* newState = new MainMenu(m_game);
			m_game.requestChangeState(*newState);
		}
		if (currentEvent.type == sf::Event::EventType::Closed) {
			m_game.requestQuit();
		}
	}

	if ((m_clock.getElapsedTime() - startTime).asSeconds() > 3) {
		IGameState* newState = new MainMenu(m_game);
		m_game.requestChangeState(*newState);
	}
}

void SplashScreen::update() {
	
}

void SplashScreen::draw() {
	m_game.mainWindow()->clear(sf::Color::Black);
	m_game.mainWindow()->draw(m_sprite);
	m_game.mainWindow()->display();
}