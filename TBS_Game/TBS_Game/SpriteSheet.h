#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <SFML/Graphics.hpp>
#include <vector>


//Use this class as a sprite/texture all in one solution for animations. 
//It owns a sprite which you can access through sprite().

class SpriteSheet
{
public:
	struct Animation {
		std::string name;
		int startFrame;
		int endFrame;
		int m_waitTicks;
		int m_loopWaitTicks;
	};

	SpriteSheet(std::string spriteSheetName);
	~SpriteSheet();
	SpriteSheet(const SpriteSheet& other);
	void operator=(const SpriteSheet& other);

	void setAnimation(const std::string animationName, bool keepPlacement);
	void resetAnimation();
	void update();

	void setSpriteSheet(std::string spriteSheetName);

	sf::Sprite& sprite();

private:
	SpriteSheet();
	
	void setSpriteToFrame(int frameNumber);

	std::string m_spriteSheetName;
	int m_frameHeight;
	int m_frameWidth; 
	int m_framesTall;
	int m_framesWide;

	std::vector<Animation> m_animations;
	int m_currentAnimation;
	int m_currentFrame;
	sf::Texture m_texture;
	std::string m_texturePath;
	sf::Sprite m_sprite;

	bool m_loop;
	bool m_waiting;
	int m_ticks;
};

#endif