#include "Hero.h"
#include "json.h"
#include "GameUtilities.h"
#include <fstream>

Hero::Hero(std::string heroName) :
	m_spriteSheet(heroName), 
	m_name(heroName) {

	m_spriteSheet.setAnimation("selected", true);

	std::ifstream unitStream("../Assets/Data/units.json", std::ios_base::binary);
	if (!unitStream.good()) {
		GameUtilities::exitWithMessage("failed to load ../Assets/Data/units.json in Hero constructor");
	}

	Json::Value root = NULL;
	unitStream >> root;
	Json::Value heroRoot;
	if (!root.isMember(heroName)) {
		GameUtilities::exitWithMessage("failed to find " + heroName + " in Hero constructor.");
	}
	heroRoot = root[heroName];
	std::string toFind = "health";
	if (!heroRoot.isMember(toFind) || !heroRoot[toFind].isInt()) {
		GameUtilities::exitWithMessage("failed to find " + toFind + " in Hero constructor.");
	}
	m_health = heroRoot[toFind].asInt();

	toFind = "strength";
	if (!heroRoot.isMember(toFind) || !heroRoot[toFind].isInt()) {
		GameUtilities::exitWithMessage("failed to find " + toFind + " in Hero constructor.");
	}
	m_strength = heroRoot[toFind].asInt();

	toFind = "armor";
	if (!heroRoot.isMember(toFind) || !heroRoot[toFind].isInt()) {
		GameUtilities::exitWithMessage("failed to find " + toFind + " in Hero constructor.");
	}
	m_armor = heroRoot[toFind].asInt();

	toFind = "intelligence";
	if (!heroRoot.isMember(toFind) || !heroRoot[toFind].isInt()) {
		GameUtilities::exitWithMessage("failed to find " + toFind + " in Hero constructor.");
	}
	m_intelligence = heroRoot[toFind].asInt();

	toFind = "evasion";
	if (!heroRoot.isMember(toFind) || !heroRoot[toFind].isInt()) {
		GameUtilities::exitWithMessage("failed to find " + toFind + " in Hero constructor.");
	}
	m_evasion = heroRoot[toFind].asInt();

	toFind = "energy";
	if (!heroRoot.isMember(toFind) || !heroRoot[toFind].isInt()) {
		GameUtilities::exitWithMessage("failed to find " + toFind + " in Hero constructor.");
	}
	m_energy = heroRoot[toFind].asInt();

	toFind = "move_points";
	if (!heroRoot.isMember(toFind) || !heroRoot[toFind].isInt()) {
		GameUtilities::exitWithMessage("failed to find " + toFind + " in Hero constructor.");
	}
	m_movePoints = heroRoot[toFind].asInt();

	m_gridx = 0;
	m_gridy = 0;
}
Hero::Hero(const Hero& other) {

}
Hero::~Hero() {}
void Hero::operator=(const Hero& other) {

}

void Hero::handleEvent(IGameState& gameState, sf::Event event) {

}

void Hero::update(IGameState& gameState) {
	m_spriteSheet.update();
}

void Hero::draw(IGameState& gameState, sf::RenderWindow& window) {
	window.draw(m_spriteSheet.sprite());
}

sf::Vector2f Hero::position() const {
	sf::Vector2f position = m_spriteSheet.getPosition();
	return sf::Vector2f(position);
}

void Hero::setPosition(const int& x, const int& y) {
	m_spriteSheet.setPosition(sf::Vector2f((float)x, (float)y));
}

sf::Vector2i Hero::gridPos() {
	return sf::Vector2i(m_gridx, m_gridy);
}
void Hero::setGridPos(const int& x, const int& y) {
	m_gridx = x;
	m_gridy = y;
}