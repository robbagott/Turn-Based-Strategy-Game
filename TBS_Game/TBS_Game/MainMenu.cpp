#include "MainMenu.h"
#include "NullGameState.h"
#include <iostream>

MainMenu::MainMenu() {
	if (!m_texture.loadFromFile("../Assets/Graphics/MainMenu.bmp")) {
		std::cerr << "No Main Menu texture loaded." << std::endl;
		exit(1);
		char x;
		std::cin >> x;
	}
	m_background.setTexture(m_texture);

	buttons.push_back(new MenuButton("New Game", "../Assets/Graphics/new_selected.bmp", "../Assets/Graphics/new_unselected.bmp", 27, 49, 26, 94));
	buttons.push_back(new MenuButton("Load Game", "../Assets/Graphics/load_selected.bmp", "../Assets/Graphics/load_unselected.bmp", 138, 49, 26, 94));
	buttons.push_back(new MenuButton("Options", "../Assets/Graphics/options_selected.bmp", "../Assets/Graphics/options_unselected.bmp", 27, 88, 26, 94));
	buttons.push_back(new MenuButton("Quit", "../Assets/Graphics/quit_selected.bmp", "../Assets/Graphics/quit_unselected.bmp", 138, 88, 26, 94));

	int m_selectedButton = 0;
	music.openFromFile("../Assets/Sounds/main_menu.wav");
	music.setLoop(true);
}

MainMenu::~MainMenu() {}

void MainMenu::init() {
	std::cout << "Entering main menu state" << std::endl;
	buttons[0]->silentSelect();
	m_selectedButton = 0;
	music.play();
}
void MainMenu::cleanup() {
	music.stop();
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
				if (m_selectedButton == 1) {
					game.requestQuit();
				}
				else if (m_selectedButton == 2) {
					game.requestQuit();
				}
				else if (m_selectedButton == 3) {
					game.requestQuit();
				}
				else if (m_selectedButton == 4) {
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
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i]->draw(*(game.mainWindow()));
	}

	game.mainWindow()->display();
}