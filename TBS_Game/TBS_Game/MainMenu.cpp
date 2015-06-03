#include "MainMenu.h"
#include "NullGameState.h"
#include "InMapState.h"
#include "GameUtilities.h"
#include <iostream>

MainMenu::MainMenu() : m_fadeInTimeInSeconds(2), m_selectedButton(0) {
	if (!m_texture.loadFromFile("../Assets/Graphics/MainMenu.bmp")) {
		GameUtilities::exitWithMessage("No Main Menu Background texture loaded.");
	}
	m_background.setTexture(m_texture);

	if (!m_blackTexture.loadFromFile("../Assets/Graphics/BlackScreen.bmp")) {
		GameUtilities::exitWithMessage("No Main Menu Black Screen texture loaded.");
	}
	m_blackSprite.setTexture(m_blackTexture);

	buttons.push_back(new MenuButton("New Game", "../Assets/Graphics/new_selected.bmp", "../Assets/Graphics/new_unselected.bmp", 27, 49, 26, 94));
	buttons.push_back(new MenuButton("Load Game", "../Assets/Graphics/load_selected.bmp", "../Assets/Graphics/load_unselected.bmp", 138, 49, 26, 94));
	buttons.push_back(new MenuButton("Options", "../Assets/Graphics/options_selected.bmp", "../Assets/Graphics/options_unselected.bmp", 27, 88, 26, 94));
	buttons.push_back(new MenuButton("Quit", "../Assets/Graphics/quit_selected.bmp", "../Assets/Graphics/quit_unselected.bmp", 138, 88, 26, 94));

	if (!m_music.openFromFile("../Assets/Sounds/main_menu.wav")) {
		std::cerr << "No Main Menu music found with the name " << "../Assets/Sounds/main_menu.wav" << std::endl;
	}
	m_music.setLoop(true);
}

MainMenu::~MainMenu() {
	for (unsigned int i = 0; i < buttons.size(); i++) {
		delete buttons[i];
	}
}

void MainMenu::init() {
	m_clock.restart();
	std::cout << "Entering main menu state" << std::endl;
	buttons[0]->silentSelect();
	m_selectedButton = 0;
	m_music.play();
}
void MainMenu::cleanup() {
	m_music.stop();
}
void MainMenu::pause() {}
void MainMenu::resume() {}

void MainMenu::handleEvents(IStateBasedGame& game) {
	sf::Event currentEvent;
	while (game.mainWindow()->pollEvent(currentEvent)) {
		if (currentEvent.type == sf::Event::EventType::KeyPressed) {
			if (currentEvent.key.code == sf::Keyboard::A || currentEvent.key.code == sf::Keyboard::Left) {
				if (m_selectedButton == 0) {
					//Do nothing due to left edge
				}
				if (m_selectedButton == 1) {
					buttons[1]->deselect();
					buttons[0]->select();
					m_selectedButton = 0;
				}
				if (m_selectedButton == 2) {
					//Do nothing due to left edge
				}
				if (m_selectedButton == 3) {
					buttons[3]->deselect();
					buttons[2]->select();
					m_selectedButton = 2;
				}
			}
			else if (currentEvent.key.code == sf::Keyboard::D || currentEvent.key.code == sf::Keyboard::Right) {
				if (m_selectedButton == 0) {
					buttons[0]->deselect();
					buttons[1]->select();
					m_selectedButton = 1;
				}
				if (m_selectedButton == 1) {
					//Do nothing due to right edge
				}
				if (m_selectedButton == 2) {
					buttons[2]->deselect();
					buttons[3]->select();
					m_selectedButton = 3;
				}
				if (m_selectedButton == 3) {
					//Do nothing due to right edge
				}
			}
			else if (currentEvent.key.code == sf::Keyboard::S || currentEvent.key.code == sf::Keyboard::Down) {
				if (m_selectedButton == 0) {
					buttons[0]->deselect();
					buttons[2]->select();
					m_selectedButton = 2;
				}
				if (m_selectedButton == 1) {
					buttons[1]->deselect();
					buttons[3]->select();
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
					buttons[2]->deselect();
					buttons[0]->select();
					m_selectedButton = 0;
				}
				if (m_selectedButton == 3) {
					buttons[3]->deselect();
					buttons[1]->select();
					m_selectedButton = 1;
				}
			}
			else if (currentEvent.key.code == sf::Keyboard::Return || currentEvent.key.code == sf::Keyboard::Space) {
				if (m_selectedButton == 0) {
					IGameState* newState = new InMapState("../Assets/Data/level_1.json");
					game.requestChangeState(*newState);
				}
				else if (m_selectedButton == 1) {
					game.requestQuit();
				}
				else if (m_selectedButton == 2) {
					game.requestQuit();
				}
				else if (m_selectedButton == 3) {
					game.requestQuit();
				}
			}

		}
		else if (currentEvent.type == sf::Event::EventType::Closed) {
			game.requestQuit();
		}
	}
}

void MainMenu::update(IStateBasedGame& game) {}

void MainMenu::draw(IStateBasedGame& game) {
	game.mainWindow()->clear(sf::Color::Black);

	game.mainWindow()->draw(m_background);
	for (unsigned int i = 0; i < buttons.size(); i++) {
		buttons[i]->draw(*(game.mainWindow()));
	}
	if (m_clock.getElapsedTime().asSeconds() < m_fadeInTimeInSeconds) {
		float percentage = (m_fadeInTimeInSeconds - m_clock.getElapsedTime().asSeconds())/ m_fadeInTimeInSeconds * 100;
		float alpha =  GameUtilities::interpolate(0.0, 255.0, percentage);
		m_blackSprite.setColor( sf::Color(255, 255, 255, alpha) );
		game.mainWindow()->draw(m_blackSprite);
	}

	game.mainWindow()->display();
}