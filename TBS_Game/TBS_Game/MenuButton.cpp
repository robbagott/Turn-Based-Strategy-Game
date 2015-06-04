#include "MenuButton.h"
#include <iostream>

MenuButton::MenuButton(std::string name, std::string selectedFilename, std::string unselectedFilename, int left, int top, int height, int width) {
	m_name = name;

	if (!m_selectedTexture.loadFromFile(selectedFilename)) {
		std::cout << "Could not load MenuButton with image: " << selectedFilename << std::endl;
		char x;
		std::cin >> x;
		exit(1);
	}
	if (!m_unselectedTexture.loadFromFile(unselectedFilename)) {
		std::cout << "Could not load MenuButton with image: " << unselectedFilename << std::endl;
		char x;
		std::cin >> x;
		exit(1);
	}
	m_sprite.setTexture(m_unselectedTexture);
	m_sprite.setPosition(left, top);

	m_isSelected = false;

	if (!m_selectSound.loadFromFile("../Assets/Sounds/menu_button.wav")) {
		std::cout << "Could not load MenuButton with sound: " << "../Assets/Sounds/menu_button.wav" << std::endl;
		char x;
		std::cin >> x;
		exit(1);
	}
	m_sound.setBuffer(m_selectSound);
}
MenuButton::MenuButton(const MenuButton& other) {
	*this = other;
}
void MenuButton::operator=(const MenuButton& other) {
	m_selectedTexture = other.m_selectedTexture;
	m_unselectedTexture = other.m_unselectedTexture;
	m_sprite = other.m_sprite;
	if (other.m_isSelected) {
		m_sprite.setTexture(m_selectedTexture);
	}
	else {
		m_sprite.setTexture(m_unselectedTexture);
	}
	m_sound = other.m_sound;
	m_selectSound = other.m_selectSound;
	m_sound.setBuffer(m_selectSound);
	m_isSelected = other.m_isSelected;
	m_name = other.m_name;
}

const sf::Vector2f& MenuButton::position() const {
	return m_sprite.getPosition();
}

sf::FloatRect MenuButton::bounds() const {
	return m_sprite.getLocalBounds();
}

std::string MenuButton::name() const {
	return m_name;
}

void MenuButton::silentSelect() {
	m_sprite.setTexture(m_selectedTexture);
	m_isSelected = true;
}

void MenuButton::select() {
	m_sprite.setTexture(m_selectedTexture);
	m_sound.play();
	m_isSelected = true;
}
void MenuButton::deselect() {
	m_sprite.setTexture(m_unselectedTexture);
	m_isSelected = false;
}

void MenuButton::draw(sf::RenderWindow& window) {
	window.draw(m_sprite);
}

bool MenuButton::isSelected() const {
	return m_isSelected;
}