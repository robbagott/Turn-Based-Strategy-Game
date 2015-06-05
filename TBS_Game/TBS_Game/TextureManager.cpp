#include "TextureManager.h"
#include "GameUtilities.h"

//Disallows multiple TextureManagers
bool TextureManager::m_instantiated = false;

TextureManager::TextureManager() {
	if (m_instantiated == true) {
		GameUtilities::exitWithMessage("Attempted to instatiate TextureManger twice");
	}
	m_instantiated = true;
}

sf::Texture& TextureManager::load(TextureID tid) {
	return sf::Texture();
}