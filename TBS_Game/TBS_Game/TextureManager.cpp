#include "TextureManager.h"
#include "GameUtilities.h"
#include <iostream>

TextureManager& TextureManager::get() {
	
	static TextureManager* instance = new TextureManager();
	return *instance;
}

TextureManager::TextureManager() {
}

TextureManager::~TextureManager() {
	//In case there are still textures allocated, delete them and report here.
	std::unordered_map<std::string, sf::Texture*>::iterator i = m_textures.begin();
	for (; i != m_textures.end(); i++) {
		std::cerr << "unfreed resource " << i->first << " freed at close of program" << std::endl;
		char x;
		std::cin >> x;
		delete i->second;
	}
}

sf::Texture*TextureManager::load(std::string filename) {
	//if the texture isn't found, allocate it and return it
	if (m_textures.count(filename) == 0) {

		sf::Texture* newTexture = new sf::Texture();
		if (!newTexture->loadFromFile(filename)) {
			//GameUtilities::exitWithMessage("Failed to load texture with path " + fileName);
			newTexture->loadFromFile("../Assets/Graphics/null_texture.png");
		}
		m_textures[filename] = newTexture;
		m_referenceCounts[filename] = 1;
		return newTexture;
	}

	//Just return it
	m_referenceCounts[filename] += 1;
	return m_textures[filename];
}

void TextureManager::free(std::string filename) {
	if (m_textures.count(filename) == 0) {
		GameUtilities::exitWithMessage("Attempt to deallocate unloaded texture with path "+ filename);
	}

	m_referenceCounts[filename] -= 1;
	if (m_referenceCounts[filename] == 0) {
		m_textures.erase(filename);
	}
}
