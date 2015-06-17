#include "Hero.h"
#include "json.h"
#include "GameUtilities.h"
#include <fstream>

Hero::Hero(std::string heroName, bool friendly) :
	m_spriteSheet(heroName), 
	m_name(heroName) {

	m_friendly = friendly; 

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

	m_gridPos = {0, 0};

	m_overlay.setCharacter(heroName, m_health, m_health);
}
Hero::Hero(const Hero& other) {
	*this = other;
}
Hero::~Hero() {}

void Hero::operator=(const Hero& other) {
	m_spriteSheet = other.m_spriteSheet;
	m_name = other.m_name;
	m_health = other.m_health;
	m_strength = other.m_strength;
	m_armor = other.m_armor;
	m_intelligence = other.m_intelligence;
	m_evasion = other.m_evasion;
	m_energy = other.m_energy;
	m_movePoints = other.m_movePoints;
	m_gridPos = other.m_gridPos;
	m_showOverlay = other.m_showOverlay;
	m_overlay = other.m_overlay;

	m_friendly = other.m_friendly;
}

void Hero::handleEvent(IGameState& gameState, sf::Event event) {

}

void Hero::update(IGameState& gameState) {
	m_spriteSheet.update();
}

void Hero::draw(IGameState& gameState, sf::RenderWindow& window) {
	window.draw(m_spriteSheet.sprite());

	if (m_showOverlay) {
		m_overlay.draw(window);
	}
}

sf::Vector2f Hero::position() const {
	sf::Vector2f position = m_spriteSheet.getPosition();
	return sf::Vector2f(position);
}

void Hero::setPosition(const int& x, const int& y) {
	m_spriteSheet.setPosition((float)x, (float)y);
}

bool Hero::friendly() const {
	return m_friendly;
}

const std::string& Hero::name() const {
	return m_name;
}
