#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>

/* Haven't yet decided to make one of these and haven't thought out how to implement it.
Regardless, it is not a critical feature in a small 2d game. 
I may come back to this if I have time later and am not too burnt out developing the core of the game.*/
class TextureManager
{
public:
	enum TextureID { TID_GRASS_1, TID_FOREST_1, TID_BLACKSCREEN, TID_MENUBG, TID_NEWGAMEBUTTON, TID_LOADGAMEBUTTON, TID_OPTIONSBUTTON, TID_QUITBUTTON };

	TextureManager();
	~TextureManager();

	sf::Texture& load(TextureID);
	void free(TextureID);


private:
	TextureManager(const TextureManager& other);
	void operator=(const TextureManager& other);

	std::map<TextureID, sf::Texture> m_textures;
	std::map<TextureID, int> m_referenceCounts;

	static bool m_instantiated;
};

#endif