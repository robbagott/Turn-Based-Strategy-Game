#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <SFML/Graphics.hpp>
#include "IGameState.h"

class IStateBasedGame;

class SplashScreen : public IGameState
{
public:
	SplashScreen(std::string filename);
	~SplashScreen();

	void init();
	void cleanup();

	void pause();
	void resume();
	
	void handleEvents(IStateBasedGame& game);
	void update(IStateBasedGame& game);
	void draw(IStateBasedGame& game);

private:
	//Can't be instantiated without a target image filename
	SplashScreen();
	//program shouldn't start calling undefined copiers under my nose!
	SplashScreen(const SplashScreen&);
	void operator=(const SplashScreen&);

	sf::Sprite m_sprite;
	sf::Texture m_texture;

	sf::Clock m_clock;
};

#endif