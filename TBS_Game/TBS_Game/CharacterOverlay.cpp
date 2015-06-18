#include "CharacterOverlay.h"
#include "AppInfo.h"
#include "GameUtilities.h"

CharacterOverlay::CharacterOverlay() :
	m_currentHealth(0),
	m_totalHealth(0),
	m_characterName("") {
	m_background = TextureManager::get().load("../Assets/Graphics/null_texture.png");
	m_profileView = TextureManager::get().load("../Assets/Graphics/null_texture.png");
	m_initialized = false;
}

CharacterOverlay::CharacterOverlay(const std::string& characterName, const int& currentHealth, const int& totalHealth) :
	m_currentHealth(currentHealth),
	m_totalHealth(totalHealth),
	m_characterName(characterName),
	m_initialized(true) {

	m_background = TextureManager::get().load("../Assets/Graphics/character_overlay.png");
	m_profileView = TextureManager::get().load("../Assets/Graphics/" + characterName + "_profile.png");

	
	m_healthBar.setSize(sf::Vector2f(m_background->getSize().x / 2, m_background->getSize().y / 5));
	m_healthBar.setFillColor(sf::Color::Black);
	
	m_currentHealthBar.setSize(sf::Vector2f(m_healthBar.getSize().x * (float) m_currentHealth/ (float)m_totalHealth, m_background->getSize().y / 5));
	m_currentHealthBar.setFillColor(sf::Color::Green);
}

CharacterOverlay::CharacterOverlay(const CharacterOverlay& other) {
	*this = other;
}

void CharacterOverlay::operator=(const CharacterOverlay& other) {
	m_characterName = other.m_characterName;

	TextureManager::get().free(m_background);
	TextureManager::get().free(m_profileView);
	m_background = TextureManager::get().load("../Assets/Graphics/character_overlay.png");
	m_profileView = TextureManager::get().load("../Assets/Graphics/" + other.m_characterName + "_profile.png");

	m_initialized = other.m_initialized;
	m_healthBar = other.m_healthBar;
	m_currentHealthBar = other.m_healthBar;
	m_currentHealth = other.m_currentHealth;
	m_totalHealth = other.m_totalHealth;
}

CharacterOverlay::~CharacterOverlay() {
	TextureManager::get().free(m_background);
	TextureManager::get().free(m_profileView);
}

void CharacterOverlay::draw(sf::RenderWindow& window) {
	if (!m_initialized) {
		GameUtilities::exitWithMessage("Attempted to draw CharacterOverlay without initializing it");
	}
	
	m_sprite.setTexture(*m_background);
	m_sprite.setPosition(AppInfo::get()->resx() / 20.0, AppInfo::get()->resy() / 15.0);
	window.draw(m_sprite);

	m_sprite.setTexture(*m_profileView);
	m_sprite.setPosition(AppInfo::get()->resx() / 20.0 + 5, AppInfo::get()->resy() / 15.0 + 5);
	window.draw(m_sprite);

	m_healthBar.setPosition(m_sprite.getPosition().x + m_background->getSize().x / 3, m_sprite.getPosition().y + m_background->getSize().y / 3);
	window.draw(m_healthBar);
	m_currentHealthBar.setPosition(m_sprite.getPosition().x + m_background->getSize().x / 3, m_sprite.getPosition().y + m_background->getSize().y / 3);
	window.draw(m_currentHealthBar);
}

void CharacterOverlay::setHealth(const int& newHealth) {
	m_currentHealth = newHealth;
	m_currentHealthBar.setSize(sf::Vector2f(m_healthBar.getSize().x * (float) m_currentHealth/ (float) m_totalHealth, m_currentHealthBar.getSize().y));
}

void CharacterOverlay::setCharacter(const std::string& characterName, const int& currentHealth, const int& totalHealth) {
	m_currentHealth = currentHealth;
	m_totalHealth = totalHealth;
	m_characterName = characterName;
	m_initialized = true;

	TextureManager::get().free(m_background);
	TextureManager::get().free(m_profileView);
	m_background = TextureManager::get().load("../Assets/Graphics/character_overlay.png");
	m_profileView = TextureManager::get().load("../Assets/Graphics/" + characterName + "_profile.png");

	m_healthBar.setSize(sf::Vector2f(m_background->getSize().x / 2, m_background->getSize().y / 5));
	m_healthBar.setFillColor(sf::Color::Black);

	m_currentHealthBar.setSize(sf::Vector2f(m_healthBar.getSize().x * (float)m_currentHealth / (float)m_totalHealth, m_background->getSize().y / 5));
	m_currentHealthBar.setFillColor(sf::Color::Green);
}