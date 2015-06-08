#include "SpriteSheet.h"
#include <fstream>
#include "json.h"
#include "GameUtilities.h"

SpriteSheet::SpriteSheet(std::string spriteSheetName, sf::Sprite& sprite ) :
	m_currentAnimation(0),
	m_currentFrame(0),
	m_sprite(sprite) {

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
	for (int i = 0; i < animationInfo.size(); i++) {
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
		toAdd.startFrame = animationInfo["end"].asInt();

		m_animations.push_back(toAdd);
	}

	sf::Vector2u textureSize = m_texture.getSize();

	m_framesTall = textureSize.y / m_frameHeight;
	m_framesWide = textureSize.x / m_frameWidth;

}

SpriteSheet::~SpriteSheet() {

} 

void SpriteSheet::setAnimation(std::string animationName) {
	int i = 0;
	for (; i < m_animations.size(); i++) {
		if (m_animations[i].name == animationName) {
			m_currentAnimation = i;
		}
	}

	if (i == m_animations.size()) {
		GameUtilities::exitWithMessage("could not set animation to " + animationName);
	}
}

void SpriteSheet::resetAnimation() {
	m_currentFrame = 0;
}

void SpriteSheet::update() {
	if (m_currentFrame == m_animations[m_currentAnimation].endFrame) {
		m_currentFrame = 0;
	}
	else {
		m_currentFrame += 1;
	}

	int row = (m_currentFrame + m_animations[m_currentAnimation].startFrame) / m_framesWide;
	int col = (m_currentFrame + m_animations[m_currentAnimation].startFrame) % m_framesWide;
	m_sprite.setTextureRect(sf::IntRect(col * m_frameWidth, row*m_frameHeight, m_frameWidth, m_frameHeight));
}
