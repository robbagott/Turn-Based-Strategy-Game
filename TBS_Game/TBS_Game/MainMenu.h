#ifndef MAINMENU_H
#define MAINMENU_H

#include <vector>
#include "IGameState.h"
#include "MenuButton.h"

class IStateBasedGame;

class MainMenu : public IGameState
{
public:
	MainMenu();
	~MainMenu();
	void init();
	void cleanup();

	void pause();
	void resume();

	void handleEvents(IStateBasedGame& game);
	void update(IStateBasedGame& game);
	void draw(IStateBasedGame& game);

private:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	
	std::vector<MenuButton> buttons;
};

#endif