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
	sf::Texture m_blackTexture;
	sf::Sprite m_blackSprite;
	sf::Music m_music;
	int m_selectedButton;
	std::vector<MenuButton*> buttons;

	sf::Clock m_clock;
	int m_fadeInTimeInSeconds;
};

#endif