#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <SFML/Graphics.hpp>
#include "IGameState.h"

class SplashScreen : public IGameState
{
public:
	SplashScreen(Game& game, std::string filename);
	~SplashScreen();

	void init();
	void cleanup();

	void pause();
	void resume();
	
	void handleEvents();
	void update();
	void draw();

private:
	//Can't be instantiated without a target image filename
	SplashScreen();
	//program shouldn't start calling undefined copiers under my nose!
	SplashScreen(const SplashScreen&);
	void operator=(const SplashScreen&);

	Game& m_game;
	sf::Sprite m_sprite;
	sf::Texture m_texture;

	sf::Clock m_clock;
};

#endif