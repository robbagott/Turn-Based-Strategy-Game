#include "SpriteSheet.h"
#include <fstream>
#include "json.h"
#include "GameUtilities.h"

SpriteSheet::SpriteSheet(std::string spriteSheetName) :
	m_currentAnimation(0),
	m_currentFrame(0),
	m_spriteSheetName(spriteSheetName),
	m_loop(true),
	m_waiting(false),
	m_ticks(0) {

	std::ifstream spriteStream("../Assets/Data/spritesheets.json", std::ios_base::binary);
	if (!spriteStream.good()) {
		GameUtilities::exitWithMessage("failed to load ../Assets/Data/spritesheets.json in InMapState constructor");
	}

	Json::Value root = NULL;
	spriteStream >> root;

	//Find sprite sheet info.
	const Json::Value spriteInfo = root[spriteSheetName];
	if (spriteInfo == NULL) {
		GameUtilities::exitWithMessage("Failed to load sprite sheet with name " + spriteSheetName);
	}
	
	//Get the texture
	std::string filepath = "";
	if (spriteInfo.isMember("filepath") && spriteInfo["filepath"].isString()) {
		filepath = spriteInfo.get("filepath", "").asString();
		if ( !m_texture.loadFromFile(filepath) ) {
			GameUtilities::exitWithMessage("failed to load texture for " + spriteSheetName);
		}
	}
	else {
		GameUtilities::exitWithMessage("failed to find texture filepath in spritesheet with name " + spriteSheetName);
	}

	//set the filepath
	m_texturePath = filepath;

	//Get the frame height
	if (spriteInfo.isMember("frame_height") && spriteInfo["frame_height"].isInt()) {
		m_frameHeight = spriteInfo["frame_height"].asInt();
	}
	else {
		GameUtilities::exitWithMessage("failed to find frame_height in spritesheet with name " + spriteSheetName);
	}

	//Get the frame width
	if (spriteInfo.isMember("frame_width") && spriteInfo["frame_width"].isInt()) {
		m_frameWidth = spriteInfo["frame_width"].asInt();
	}
	else {
		GameUtilities::exitWithMessage("failed to find frame_width in spritesheet with name " + spriteSheetName);
	}

	//get the animations
	Animation toAdd;
	Json::Value animationInfo = spriteInfo["animations"];
	for (unsigned int i = 0; i < animationInfo.size(); i++) {
		if (!animationInfo[i].isMember("name") || !animationInfo[i]["name"].isString()) {
			GameUtilities::exitWithMessage("failed to load name of animation in spritesheet with name " + spriteSheetName);
		}
		toAdd.name = animationInfo[i]["name"].asString();

		if (!animationInfo[i].isMember("start") || !animationInfo[i]["start"].isInt()) {
			GameUtilities::exitWithMessage("failed to load start of animation in spritesheet with name " + spriteSheetName);
		}
		toAdd.startFrame = animationInfo[i]["start"].asInt();

		if (!animationInfo[i].isMember("end") || !animationInfo[i]["end"].isInt()) {
			GameUtilities::exitWithMessage("failed to load end of animation in spritesheet with name " + spriteSheetName);
		}
		toAdd.endFrame = animationInfo[i]["end"].asInt();

		if (!animationInfo[i].isMember("wait_ticks") || !animationInfo[i]["wait_ticks"].isInt()) {
			GameUtilities::exitWithMessage("failed to load wait ticks of animation in spritesheet with name " + spriteSheetName);
		}
		toAdd.m_waitTicks = animationInfo[i]["wait_ticks"].asInt();

		if (!animationInfo[i].isMember("loop_wait_ticks") || !animationInfo[i]["loop_wait_ticks"].isInt()) {
			GameUtilities::exitWithMessage("failed to load loop wait ticks of animation in spritesheet with name " + spriteSheetName);
		}
		toAdd.m_loopWaitTicks = animationInfo[i]["loop_wait_ticks"].asInt();

		m_animations.push_back(toAdd);
	}

	//set the number of frames that are in texture
	sf::Vector2u textureSize = m_texture.getSize();
	m_framesTall = textureSize.y / m_frameHeight;
	m_framesWide = textureSize.x / m_frameWidth;

	//set sprite attributes
	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_frameWidth, m_frameHeight));
}

SpriteSheet::SpriteSheet(const SpriteSheet& other) {
	*this = other;
}

void SpriteSheet::operator=(const SpriteSheet& other) {
	m_frameHeight = other.m_frameHeight;
	m_frameWidth = other.m_frameWidth;
	m_animations = other.m_animations;
	m_currentAnimation = other.m_currentAnimation;
	m_currentFrame = other.m_currentFrame;
	m_framesTall = other.m_framesTall;
	m_framesWide = other.m_framesWide;
	m_spriteSheetName = other.m_spriteSheetName;
	m_texturePath = other.m_texturePath;
	m_loop = other.m_loop;
	m_ticks = 0;

	//sprites have a shitty shallow copy pretty much
	if (!m_texture.loadFromFile(m_texturePath)) {
		GameUtilities::exitWithMessage("Failed to load texture " + m_texturePath + " in SpriteSheet copy constructor");
	}
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(other.m_sprite.getPosition());
	m_sprite.setTextureRect(other.m_sprite.getTextureRect());
}

SpriteSheet::~SpriteSheet() {

} 

void SpriteSheet::setAnimation(std::string animationName, bool keepPlacement) {
	//Find new animation
	bool found = false;
	for (unsigned int i = 0; i < m_animations.size(); i++) {
		if (m_animations[i].name == animationName) {
			m_currentAnimation = i;
			found = true;
		}
	}

	if (!found) {
		GameUtilities::exitWithMessage("could not set animation to " + animationName);
	}

	//update sprite to reflect
	if (!keepPlacement) {
		setSpriteToFrame(0);
	}
	else {
		setSpriteToFrame(m_currentFrame);
	}
}

void SpriteSheet::resetAnimation() {
	m_currentFrame = 0;
	m_ticks = 0;
}

void SpriteSheet::update() {
	if (m_waiting) {
		if (m_ticks > m_animations[m_currentAnimation].m_loopWaitTicks) {
			m_waiting = false;
		}	
	}
	//if the amount of ticks between frames of animation has passed
	else if (m_ticks > m_animations[m_currentAnimation].m_waitTicks) {
		//update frame, set to wait if applicable
		if (m_currentFrame == m_animations[m_currentAnimation].endFrame - m_animations[m_currentAnimation].startFrame) {
			m_currentFrame = 0;
			if (m_animations[m_currentAnimation].m_loopWaitTicks > 0) {
				m_waiting = true;
			}
		}
		else {
			m_currentFrame += 1;
		}

		//calculate new sprite position
		int row = (m_currentFrame + m_animations[m_currentAnimation].startFrame) / m_framesWide;
		int col = (m_currentFrame + m_animations[m_currentAnimation].startFrame) % m_framesWide;
		m_sprite.setTextureRect(sf::IntRect(col * m_frameWidth, row*m_frameHeight, m_frameWidth, m_frameHeight));

		//reset ticks
		m_ticks = 0;
	}

	++m_ticks;
}

void SpriteSheet::setSpriteSheet(std::string spriteSheetName) {
		m_currentAnimation = 0;
		m_currentFrame = 0;
		m_spriteSheetName = spriteSheetName;
		m_ticks = 0;

		std::ifstream spriteStream("../Assets/Data/spritesheets.json", std::ios_base::binary);
		if (!spriteStream.good()) {
			GameUtilities::exitWithMessage("failed to load ../Assets/Data/spritesheets.json in InMapState constructor");
		}

		Json::Value root = NULL;
		spriteStream >> root;

		//Find sprite sheet info.
		const Json::Value spriteInfo = root[spriteSheetName];
		if (spriteInfo == NULL) {
			GameUtilities::exitWithMessage("Failed to load sprite sheet with name " + spriteSheetName);
		}

		//Get the texture
		std::string filepath = "";
		if (spriteInfo.isMember("filepath") && spriteInfo["filepath"].isString()) {
			filepath = spriteInfo.get("filepath", "").asString();
			if (!m_texture.loadFromFile(filepath)) {
				GameUtilities::exitWithMessage("failed to load texture for " + spriteSheetName);
			}
		}
		else {
			GameUtilities::exitWithMessage("failed to find texture filepath in spritesheet with name " + spriteSheetName);
		}

		//set texture path
		m_texturePath = filepath;

		//Get the frame height
		if (spriteInfo.isMember("frame_height") && spriteInfo["frame_height"].isInt()) {
			m_frameHeight = spriteInfo["frame_height"].asInt();
		}
		else {
			GameUtilities::exitWithMessage("failed to find frame_height in spritesheet with name " + spriteSheetName);
		}

		//Get the frame width
		if (spriteInfo.isMember("frame_width") && spriteInfo["frame_width"].isInt()) {
			m_frameHeight = spriteInfo["frame_width"].asInt();
		}
		else {
			GameUtilities::exitWithMessage("failed to find frame_width in spritesheet with name " + spriteSheetName);
		}

		//get the animations
		Animation toAdd;
		Json::Value animationInfo = spriteInfo["animations"];
		for (unsigned int i = 0; i < animationInfo.size(); i++) {
			if (!animationInfo[i].isMember("name") || !animationInfo[i]["name"].isString()) {
				GameUtilities::exitWithMessage("failed to load name of animation in spritesheet with name " + spriteSheetName);
			}
			toAdd.name = animationInfo[i]["name"].asString();

			if (!animationInfo[i].isMember("start") || !animationInfo[i]["start"].isInt()) {
				GameUtilities::exitWithMessage("failed to load start of animation in spritesheet with name " + spriteSheetName);
			}
			toAdd.startFrame = animationInfo["start"].asInt();

			if (!animationInfo[i].isMember("end") || !animationInfo[i]["end"].isInt()) {
				GameUtilities::exitWithMessage("failed to load end of animation in spritesheet with name " + spriteSheetName);
			}
			toAdd.endFrame = animationInfo["end"].asInt();

			m_animations.push_back(toAdd);
		}

		sf::Vector2u textureSize = m_texture.getSize();

		m_framesTall = textureSize.y / m_frameHeight;
		m_framesWide = textureSize.x / m_frameWidth;

		m_sprite.setTexture(m_texture);
}

sf::Sprite& SpriteSheet::sprite()  {
	return m_sprite;
}

//frameNumber is the frame in the current animation
void SpriteSheet::setSpriteToFrame(int frameNumber) {
	m_currentFrame = frameNumber;

	int row = (m_currentFrame + m_animations[m_currentAnimation].startFrame) / m_framesWide;
	int col = (m_currentFrame + m_animations[m_currentAnimation].startFrame) % m_framesWide;
	m_sprite.setTextureRect(sf::IntRect(col * m_frameWidth, row * m_frameHeight, m_frameWidth, m_frameHeight));
}