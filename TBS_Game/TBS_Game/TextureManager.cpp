#include "TextureManager.h"
#include "GameUtilities.h"

//Disallows multiple TextureManagers
bool TextureManager::m_instantiated = false;

TextureManager::TextureManager() {
	if (m_instantiated) {
		GameUtilities::exitWithMessage("Attempted to instantiate TextureManger twice");
	}
	m_instantiated = true;

}

TextureManager::~TextureManager() {
	std::unordered_map<std::string, sf::Texture*>::iterator i = m_textures.begin();
	for (; i != m_textures.end(); i++) {
		delete i->second;
	}
}

sf::Texture& TextureManager::load(std::string filename) {
	//if the texture isn't found, allocate it and return it
	if (m_textures.count(filename) == 0) {

		std::string fullPath = "../Assets/Graphics/" + filename + ".png";
		sf::Texture* newTexture = new sf::Texture();
		if (!newTexture->loadFromFile(fullPath)) {
			GameUtilities::exitWithMessage("Failed to load texture with path " + fullPath);
		}
		m_textures[filename] = newTexture;
		m_referenceCounts[filename] = 1;
		return *newTexture;
	}

	//Just return it
	m_referenceCounts[filename] += 1;
	return *m_textures[filename];
}

void TextureManager::free(std::string filename) {
	if (m_textures.count(filename) == 0) {
		std::string fullPath = "../Assets/Graphics/" + filename + ".png";
		GameUtilities::exitWithMessage("Attempt to deallocate unloaded texture with path "+ fullPath);
	}

	m_referenceCounts[filename] -= 1;
	if (m_referenceCounts[filename] == 0) {
		m_textures.erase(filename);
	}
}
