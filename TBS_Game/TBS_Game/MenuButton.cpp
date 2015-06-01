#include "MenuButton.h"
#include <iostream>

MenuButton::MenuButton(std::string selectedFilename, std::string unselectedFilename, int left, int top, int height, int width) {
	if (!m_selectedTexture.loadFromFile(selectedFilename)) {
		std::cout << "Could not load MenuButton with image: " << selectedFilename << std::endl;
	}
	if (!m_selectedTexture.loadFromFile(unselectedFilename)) {
		std::cout << "Could not load MenuButton with image: " << unselectedFilename << std::endl;
	}
	m_sprite.setTexture(m_selectedTexture);
}

int MenuButton::left() const {
	return m_left;
}
int MenuButton::top() const {
	return m_top;
}
int MenuButton::height() const {
	return m_height;
}
int MenuButton::width() const {
	return m_width;
}
int MenuButton::bottom() const {
	return m_height + m_top;
}
int MenuButton::right() const {
	return m_width + m_left;
}