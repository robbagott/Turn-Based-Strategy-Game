#ifndef GAME_H
#define GAME_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "IStateBasedGame.h"
#include "AppInfo.h"
#include <stack>

class IGameState;

//GameManager can only be instantiated once
class Game : public IStateBasedGame
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
	//Any code that has access to the game engine can ask for it's window in order to draw things or perform other operations. This might be a bad idea in retrospect...
	sf::RenderWindow* mainWindow() const;

private:

	//Called when started
	void init();
	//Called when exiting
	void cleanup();
	//One game loop iteration
	void gameLoop();
	//redirects to game state. Opportunity to decorate the call
	void handleEvents();
	//Same
	void update();
	//Same
	void draw();
	//Creates the game window with settings found in the Appinfo object. 
	void initializeWindow();
	//Actually swaps in the next requested GameState object. private for a reason.
	void swapState();

	static bool m_isInstantiated;
	bool m_quitRequested;
	bool m_changeStateRequested;
	std::stack<IGameState*> m_gameStates;
	IGameState* m_nextGameState;
	sf::RenderWindow* m_mainWindow;
	AppInfo* m_appInfo;
};

#endif