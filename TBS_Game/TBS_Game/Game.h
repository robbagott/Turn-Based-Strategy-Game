#ifndef GAME_H
#define GAME_H

#include <cassert>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "IStateBasedGame.h"
#include "AppInfo.h"

class IGameState;

//GameManager can only be instantiated once
class Game : public IStateBasedGame
{
public:
	Game();
	~Game();

	void start();
	void requestChangeState(IGameState& gameState);
	void requestQuit();
	sf::RenderWindow* mainWindow() const;

private:
	void init();
	void cleanup();
	void gameLoop();
	void handleEvents();
	void update();
	void draw();
	void initializeWindow();
	void swapState();

	static bool m_isInstantiated;
	bool m_quitRequested;
	bool m_changeStateRequested;
	IGameState* m_gameState;
	IGameState* m_nextGameState;
	sf::RenderWindow* m_mainWindow;
	AppInfo* m_appInfo;
};

#endif