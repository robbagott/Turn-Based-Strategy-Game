#include "MenuButton.h"
#include "GameUtilities.h"
#include <iostream>

MenuButton::MenuButton(std::string spriteSheetName, int left, int top, int height, int width) :
	m_spriteSheet(spriteSheetName) {

	m_spriteSheet.setAnimation("unselected", true);
	m_spriteSheet.sprite().setPosition(left, top);

	m_isSelected = false;

	if (!m_selectSound.loadFromFile("../Assets/Sounds/menu_button.wav")) {
		GameUtilities::exitWithMessage("Could not load MenuButton with sound: ../Assets/Sounds/menu_button.wav");
	}
	m_sound.setBuffer(m_selectSound);
}

MenuButton::MenuButton(const MenuButton& other) :
	m_spriteSheet(other.m_spriteSheet) {
	*this = other;
}
void MenuButton::operator=(const MenuButton& other) {
	m_spriteSheet = other.m_spriteSheet;
	if (other.m_isSelected) {
		m_spriteSheet.setAnimation("selected", true);
	}
	else {
		m_spriteSheet.setAnimation("unselected", true);
	}
	m_sound = other.m_sound;
	m_selectSound = other.m_selectSound;
	m_sound.setBuffer(m_selectSound);
	m_isSelected = other.m_isSelected;
}

sf::Vector2f MenuButton::position() {
	return m_spriteSheet.sprite().getPosition();
}

sf::FloatRect MenuButton::bounds() {
	return m_spriteSheet.sprite().getLocalBounds();
}

void MenuButton::silentSelect() {
	m_spriteSheet.setAnimation("selected", true);
	m_isSelected = true;
}

void MenuButton::select() {
	m_spriteSheet.setAnimation("selected", true);
	m_sound.play();
	m_isSelected = true;
}
void MenuButton::deselect() {
	m_spriteSheet.setAnimation("unselected", true);
	m_isSelected = false;
}

void MenuButton::update() {
	m_spriteSheet.update();
}

void MenuButton::draw(sf::RenderWindow& window) {
	window.draw(m_spriteSheet.sprite());
}

bool MenuButton::isSelected() const {
	return m_isSelected;
}