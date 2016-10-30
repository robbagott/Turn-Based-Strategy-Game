#include "Game.h"
#include "SplashScreen.h"
#include "MainMenu.h"
#include "NullGameState.h"
#include "GameUtilities.h"
#include <iostream>

bool Game::m_isInstantiated = false;

Game::Game() {
	//There can only be one
	if (Game::m_isInstantiated) {
		GameUtilities::exitWithMessage("Attempted to instatiate a second instance of Game.");
	}
	Game::m_isInstantiated = true;
	Game::m_quitRequested = false;
	Game::m_changeStateRequested = false;

	m_gameStates.push(new NullGameState(*this));
	m_nextGameState = new NullGameState(*this);

	m_frameTime = sf::seconds(1.0f/60.0f);
}

Game::~Game() {
	delete m_mainWindow;
	while(m_gameStates.size() != 0) {
		delete m_gameStates.top();
		m_gameStates.pop();
	}
	delete m_nextGameState;

	TextureManager::get().freeAll();
}

void Game::init() {
	//initialize configuration
	AppInfo::get();
	initializeWindow();
	m_mainWindow->clear(sf::Color::Black);
	m_mainWindow->display();

	if (AppInfo::get()->splashEnabled()) {
		requestChangeState(*(new SplashScreen(*this, "../Assets/Graphics/SplashScreen.bmp")));
		swapState();
	}
	else {
		requestChangeState(*(new MainMenu(*this)));
		swapState();
	}
}

void Game::start() {

	init();

	//counter for printing framerate
	int counter = 0;
	float avgFrameTimeUsed = 0;

	sf::Time startTime, endTime;
	while (!m_quitRequested) {
		startTime = m_loopClock.getElapsedTime();
		gameLoop();
		endTime = m_loopClock.getElapsedTime();
		sf::sleep(m_frameTime - (endTime - startTime));

		//Track percentage of frametime used
		avgFrameTimeUsed = (avgFrameTimeUsed + (100 * (float)(endTime - startTime).asMicroseconds() / (float)(m_loopClock.getElapsedTime() - startTime).asMicroseconds())) / 2;
		counter++;
		if (counter >= 60) {
			//Displays percentage of used time per frame.
			std::cout << "Percentage frame time use: " << avgFrameTimeUsed << std::endl;
			counter = 0;
			avgFrameTimeUsed = 0;
		}
	}

	cleanup();
}

void Game::gameLoop() {
	handleEvents();
	update();
	draw();

	if (m_changeStateRequested) {
		swapState();
	}
}

void Game::initializeWindow() {
	if (AppInfo::get()->screenMode() == AppInfo::SM_FULLSCREEN) {
		//Create window
		m_mainWindow = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "TBS_Game", sf::Style::Fullscreen);

		//Handle stretching/scaline
		if (AppInfo::get()->stretchToFitEnabled()) {
			m_mainWindow->setView(sf::View(sf::FloatRect(0.0, 0.0, (float)AppInfo::get()->resx(), (float)AppInfo::get()->resy())));
		}
		else {
			int fullWidth = sf::VideoMode::getDesktopMode().width;
			int fullHeight = sf::VideoMode::getDesktopMode().height;
			int maxScaling = std::min(fullWidth / AppInfo::get()->resx(), fullHeight / AppInfo::get()->resy());
			int xOffset = (fullWidth - AppInfo::get()->resx()*maxScaling) / (2 * maxScaling);
			int yOffset = (fullHeight - AppInfo::get()->resy()*maxScaling) / (2 * maxScaling);
			m_mainWindow->setView(sf::View(sf::FloatRect((float)-xOffset, (float)-yOffset, (float)AppInfo::get()->resx() + 2 * xOffset, (float)AppInfo::get()->resy() + 2 * yOffset)));
		}
	}
	else if (AppInfo::get()->screenMode() == AppInfo::SM_BORDERLESSWINDOW) {
		//Create window
		m_mainWindow = new sf::RenderWindow(sf::VideoMode(AppInfo::get()->windowWidth(), AppInfo::get()->windowHeight()), "TBS_Game", sf::Style::None);

		//Handle stretching/scaling
		if (AppInfo::get()->stretchToFitEnabled()) {
			m_mainWindow->setView(sf::View(sf::FloatRect(0.0, 0.0, (float)AppInfo::get()->resx(), (float)AppInfo::get()->resy())));
		}
		else {
			int fullWidth = sf::VideoMode::getDesktopMode().width;
			int fullHeight = sf::VideoMode::getDesktopMode().height;
			int maxScaling = std::min(fullWidth / AppInfo::get()->resx(), fullHeight / AppInfo::get()->resy());
			int xOffset = (fullWidth - AppInfo::get()->resx()*maxScaling) / (2 * maxScaling);
			int yOffset = (fullHeight - AppInfo::get()->resy()*maxScaling) / (2 * maxScaling);
			m_mainWindow->setView(sf::View(sf::FloatRect((float)-xOffset, (float)-yOffset, (float)AppInfo::get()->resx() + 2 * xOffset, (float)AppInfo::get()->resy() + 2 * yOffset)));
		}
	}
	else {
		//Create Window
		m_mainWindow = new sf::RenderWindow(sf::VideoMode(AppInfo::get()->windowWidth(), AppInfo::get()->windowHeight()), "TBS_Game");

		//Handle stretching/scaling
		if (AppInfo::get()->stretchToFitEnabled()) {
			m_mainWindow->setView(sf::View(sf::FloatRect(0.0, 0.0, (float)AppInfo::get()->resx(), (float)AppInfo::get()->resy())));
		}
		else {
			std::cout << (float)AppInfo::get()->windowWidth() / AppInfo::get()->resx() << std::endl;
			float maxScaling = std::min((float) AppInfo::get()->windowWidth() / AppInfo::get()->resx(), (float) AppInfo::get()->windowHeight() / AppInfo::get()->resy());
			int xOffset = (AppInfo::get()->windowWidth() - AppInfo::get()->resx()*maxScaling) / (2 * maxScaling);
			int yOffset = (AppInfo::get()->windowHeight() - AppInfo::get()->resy()*maxScaling) / (2 * maxScaling);
			m_mainWindow->setView(sf::View(sf::FloatRect((float)-xOffset, (float)-yOffset, (float)AppInfo::get()->resx() + 2 * xOffset, (float)AppInfo::get()->resy() + 2 * yOffset)));
		}
	}

	m_mainWindow->setMouseCursorVisible(false);
}

void Game::requestQuit() {
	m_quitRequested = true;
}

void Game::cleanup() {

}

sf::RenderWindow* Game::mainWindow() const {
	return m_mainWindow;
}

void Game::requestChangeState(IGameState& newState) {
	delete m_nextGameState;
	m_nextGameState = &newState;
	m_changeStateRequested = true;
}

void Game::requestPushState(IGameState& newState) {}

void Game::requestPopState(IGameState& newState) {}

void Game::swapState() {
	if (!m_changeStateRequested) {
		exit(1);
	}

	m_gameStates.top()->cleanup();
	delete m_gameStates.top();
	m_gameStates.pop();
	m_gameStates.push(m_nextGameState);

	//Reset nextGameState to NullGameState. 
	//We don't want to accidentally deallocate the current gameState by deallocating nextGameState
	m_nextGameState = new NullGameState(*this);
	m_gameStates.top()->init();

	m_changeStateRequested = false;
}

void Game::handleEvents() {
	m_gameStates.top()->handleEvents();
}

void Game::update() {
	m_gameStates.top()->update();
}

void Game::draw() {
	m_gameStates.top()->draw();
}