#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <SFML/Graphics.hpp>
#include <vector>

class SpriteSheet
{
public:
	struct Animation {
		std::string name;
		int startFrame;
		int endFrame;
	};

	SpriteSheet(const std::string spriteSheetName, sf::Sprite& sprite);
	~SpriteSheet();

	void setAnimation(const std::string animationName);
	void resetAnimation();
	void update();

private:
	SpriteSheet(const SpriteSheet& other);
	void operator=(const SpriteSheet& other);

	int m_frameHeight;
	int m_frameWidth;
	std::vector<Animation> m_animations;
	int m_currentAnimation;
	int m_currentFrame;
	sf::Texture m_texture;
	sf::Sprite& m_sprite;

	int m_framesTall;
	int m_framesWide;
};

#endif