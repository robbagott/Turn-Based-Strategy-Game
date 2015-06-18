#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>

//TextureManager is a singleton class. It actually makes a lot of sense here.
//We truly do want global access (Much inconvenience and tons of TextureMgr parameter passing if not) and we only
//want one. 
class TextureManager
{
public:
	static TextureManager& get();
	~TextureManager();

	sf::Texture* load(std::string);
	void free(std::string);
	void free(sf::Texture* texture);
	void freeAll();

private:
	TextureManager();
	TextureManager(const TextureManager& other);
	void operator=(const TextureManager& other);

	std::unordered_map<std::string, sf::Texture*> m_textures;
	std::unordered_map<std::string, int> m_referenceCounts;
	static bool m_instantiated;
};

#endif