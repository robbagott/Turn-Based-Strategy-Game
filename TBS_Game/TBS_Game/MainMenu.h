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
	sf::Sprite m_background;
	sf::Texture m_texture;
	sf::Music music;
	int m_selectedButton;
	std::vector<MenuButton*> buttons;
};

#endif