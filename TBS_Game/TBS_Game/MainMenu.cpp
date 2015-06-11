#include "MainMenu.h"
#include "NullGameState.h"
#include "InMapState.h"
#include "GameUtilities.h"
#include <iostream>

MainMenu::MainMenu(Game& game) : 
	m_game(game),
	m_fadeInTimeInSeconds(2), 
	m_fadeOutTimeInSeconds(4), 
	m_selectedButton(0), 
	m_newGamePushed(false),
	m_transitionNewGame(false),
	m_responsive(true) {

	if (!m_texture.loadFromFile("../Assets/Graphics/main_menu.png")) {
		GameUtilities::exitWithMessage("No Main Menu Background texture loaded.");
	}
	m_background.setTexture(m_texture);

	if (!m_blackTexture.loadFromFile("../Assets/Graphics/black_screen.png")) {
		GameUtilities::exitWithMessage("No Main Menu Black Screen texture loaded.");
	}
	m_blackSprite.setTexture(m_blackTexture);

	m_buttons.push_back(MenuButton("new_game_button", 54, 98, 52, 188));
	m_buttons.push_back(MenuButton("load_game_button", 276, 98, 52, 188));
	m_buttons.push_back(MenuButton("options_button", 54, 176, 52, 188));
	m_buttons.push_back(MenuButton("quit_button", 276, 176, 52, 188));

	if (!m_music.openFromFile("../Assets/Sounds/main_menu.wav")) {
		std::cerr << "No Main Menu music found with the name " << "../Assets/Sounds/main_menu.wav" << std::endl;
	}
	m_music.setLoop(true);

	if (!m_newGameSoundBuffer.loadFromFile("../Assets/Sounds/new_game.wav")) {
		std::cerr << "No new game sound found with the name " << "../Assets/Sounds/new_game.wav" << std::endl;
	}
	m_newGameSound.setBuffer(m_newGameSoundBuffer);
}

MainMenu::~MainMenu() {
}

void MainMenu::init() {
	m_fadeInClock.restart();
	m_buttons[0].silentSelect();
	m_selectedButton = 0;
	m_music.play();
}
void MainMenu::cleanup() {
	m_music.stop();
}
void MainMenu::pause() {}
void MainMenu::resume() {}

void MainMenu::handleEvents() {
	sf::Event currentEvent;
	while (m_game.mainWindow()->pollEvent(currentEvent)) {
		if (m_responsive) {
			if (currentEvent.type == sf::Event::EventType::KeyPressed) {
				if (currentEvent.key.code == sf::Keyboard::A || currentEvent.key.code == sf::Keyboard::Left) {
					if (m_selectedButton == 0) {
						//Do nothing due to left edge
					}
					if (m_selectedButton == 1) {
						m_buttons[1].deselect();
						m_buttons[0].select();
						m_selectedButton = 0;
					}
					if (m_selectedButton == 2) {
						//Do nothing due to left edge
					}
					if (m_selectedButton == 3) {
						m_buttons[3].deselect();
						m_buttons[2].select();
						m_selectedButton = 2;
					}
				}
				else if (currentEvent.key.code == sf::Keyboard::D || currentEvent.key.code == sf::Keyboard::Right) {
					if (m_selectedButton == 0) {
						m_buttons[0].deselect();
						m_buttons[1].select();
						m_selectedButton = 1;
					}
					if (m_selectedButton == 1) {
						//Do nothing due to right edge
					}
					if (m_selectedButton == 2) {
						m_buttons[2].deselect();
						m_buttons[3].select();
						m_selectedButton = 3;
					}
					if (m_selectedButton == 3) {
						//Do nothing due to right edge
					}
				}
				else if (currentEvent.key.code == sf::Keyboard::S || currentEvent.key.code == sf::Keyboard::Down) {
					if (m_selectedButton == 0) {
						m_buttons[0].deselect();
						m_buttons[2].select();
						m_selectedButton = 2;
					}
					if (m_selectedButton == 1) {
						m_buttons[1].deselect();
						m_buttons[3].select();
						m_selectedButton = 3;
					}
					if (m_selectedButton == 2) {
						//Do nothing due to bottom edge
					}
					if (m_selectedButton == 3) {
						//Do nothing due to bottom edge
					}
				}
				else if (currentEvent.key.code == sf::Keyboard::W || currentEvent.key.code == sf::Keyboard::Up) {
					if (m_selectedButton == 0) {
						//Do nothing due to top edge
					}
					if (m_selectedButton == 1) {
						//Do nothing due to top edge
					}
					if (m_selectedButton == 2) {
						m_buttons[2].deselect();
						m_buttons[0].select();
						m_selectedButton = 0;
					}
					if (m_selectedButton == 3) {
						m_buttons[3].deselect();
						m_buttons[1].select();
						m_selectedButton = 1;
					}
				}
				else if (currentEvent.key.code == sf::Keyboard::Return || currentEvent.key.code == sf::Keyboard::Space) {
					if (m_selectedButton == 0) {
						onNewGamePushed();
					}
					else if (m_selectedButton == 1) {
					}
					else if (m_selectedButton == 2) {
					}
					else if (m_selectedButton == 3) {
						m_game.requestQuit();
					}
				}
			}

		}
		if (currentEvent.type == sf::Event::EventType::Closed) {
			m_game.requestQuit();
		}
	}
}

void MainMenu::update() {
	if (m_newGamePushed && m_fadeOutClock.getElapsedTime().asSeconds() > m_fadeOutTimeInSeconds) {
		m_music.stop();

		IGameState* newState = new InMapState(m_game, "../Assets/Data/level_1.json");
		m_game.requestChangeState(*newState);
	}

	for (int i = 0; i < m_buttons.size(); i++) {
		m_buttons[i].update();
	}
}

void MainMenu::draw() {
	m_game.mainWindow()->clear(sf::Color::Black);

	m_game.mainWindow()->draw(m_background);
	for (unsigned int i = 0; i < m_buttons.size(); i++) {
		m_buttons[i].draw(*(m_game.mainWindow()));
	}

	if (m_newGamePushed) {
		m_fadeOutClock.getElapsedTime();
		float percentage = (m_fadeOutTimeInSeconds - m_fadeOutClock.getElapsedTime().asSeconds()) / m_fadeOutTimeInSeconds * 100;
		float alpha = GameUtilities::interpolate(255.0, 0.0, percentage);
		m_blackSprite.setColor(sf::Color(255, 255, 255, alpha));
		m_game.mainWindow()->draw(m_blackSprite);
	}
	else if (m_fadeInClock.getElapsedTime().asSeconds() < m_fadeInTimeInSeconds) {
		float percentage = (m_fadeInTimeInSeconds - m_fadeInClock.getElapsedTime().asSeconds())/ m_fadeInTimeInSeconds * 100;
		float alpha =  GameUtilities::interpolate(0.0, 255.0, percentage);
		m_blackSprite.setColor( sf::Color(255, 255, 255, alpha) );
		m_game.mainWindow()->draw(m_blackSprite);
	}

	m_game.mainWindow()->display();
}

void MainMenu::onNewGamePushed() {
	m_newGamePushed = true;
	m_fadeOutClock.restart();
	m_responsive = false;
	m_newGameSound.play();
}