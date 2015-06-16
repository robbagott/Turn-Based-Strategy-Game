#ifndef GAME_H
#define GAME_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "AppInfo.h"
#include "TextureManager.h"
#include <stack>

class IGameState;

//GameManager can only be instantiated once
class Game
{
public:
	Game();
	~Game();

	//Called by outside code to start the game
	void start();
	//In a state based game, states request state changes from the game engine in order to transition states
	void requestChangeState(IGameState& gameState);
	void requestPushState(IGameState& gameState);
	void requestPopState(IGameState& gamestate);
	//Any code that has access to the game engine can ask it nicely to quit
	void requestQuit();
	sf::RenderWindow* mainWindow() const;

private:
	//Prevent copying of the object (ensure single copy exists)
	Game(const Game&) {}
	void operator=(const Game& other);
	
	void init();
	void cleanup();
	//One game loop iteration
	void gameLoop();
	void handleEvents();
	void update();
	void draw();
	//Creates the game window with settings found in the Appinfo object. 
	void initializeWindow();
	void swapState();

	static bool m_isInstantiated;
	bool m_quitRequested;
	bool m_changeStateRequested;
	std::stack<IGameState*> m_gameStates;
	IGameState* m_nextGameState;
	sf::RenderWindow* m_mainWindow;

	sf::Clock m_loopClock;
	sf::Time m_frameTime;
};

#endif