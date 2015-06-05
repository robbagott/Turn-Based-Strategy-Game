#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SFML/Graphics.hpp>
#include "TexturePaths.h"
#include <unordered_map>

/* Haven't yet decided to make one of these and haven't thought out how to implement it.
Regardless, it is not a critical feature in a small 2d game. 
I may come back to this if I have time later and am not too burnt out developing the core of the game.*/
class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	sf::Texture& load(std::string);
	void free(std::string);


private:
	TextureManager(const TextureManager& other);
	void operator=(const TextureManager& other);

	std::unordered_map<std::string, sf::Texture> m_textures;
	std::unordered_map<std::string, int> m_referenceCounts;
	static bool m_instantiated;
};

#endif