#ifndef TEXTURE_H
#define TEXTURE_H

#include <SFML/Graphics.hpp>

class Texture
{
public:

private:
	sf::Texture* texture;
};

typedef std::shared_ptr<Texture> TexturePtr;

#endif