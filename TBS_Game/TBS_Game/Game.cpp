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
	initializeWindow();
	m_mainWindow->clear(sf::Color::Black);
	m_mainWindow->display();

	if (m_appInfo.splashEnabled()) {
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

	sf::Time startTime, endTime;
	while (!m_quitRequested) {
		startTime = m_loopClock.getElapsedTime();
		gameLoop();
		endTime = m_loopClock.getElapsedTime();
		sf::sleep(m_frameTime - (endTime - startTime));
		//std::cout << 1/(m_loopClock.getElapsedTime() - startTime).asSeconds() << std::endl;
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
	if (m_appInfo.screenMode() == AppInfo::SM_FULLSCREEN) {
		//Create window
		m_mainWindow = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "TBS_Game", sf::Style::Fullscreen);

		//Handle stretching/scaline
		if (m_appInfo.stretchToFitEnabled()) {
			m_mainWindow->setView(sf::View(sf::FloatRect(0.0, 0.0, (float)m_appInfo.resx(), (float)m_appInfo.resy())));
		}
		else {
			int fullWidth = sf::VideoMode::getDesktopMode().width;
			int fullHeight = sf::VideoMode::getDesktopMode().height;
			int maxScaling = std::min(fullWidth / m_appInfo.resx(), fullHeight / m_appInfo.resy());
			int xOffset = (fullWidth - m_appInfo.resx()*maxScaling) / (2 * maxScaling);
			int yOffset = (fullHeight - m_appInfo.resy()*maxScaling) / (2 * maxScaling);
			m_mainWindow->setView(sf::View(sf::FloatRect((float)-xOffset, (float)-yOffset, (float)m_appInfo.resx() + 2 * xOffset, (float)m_appInfo.resy() + 2 * yOffset)));
		}
	}
	else if (m_appInfo.screenMode() == AppInfo::SM_BORDERLESSWINDOW) {
		//Create window
		m_mainWindow = new sf::RenderWindow(sf::VideoMode(m_appInfo.windowWidth(), m_appInfo.windowHeight()), "TBS_Game", sf::Style::None);

		//Handle stretching/scaling
		if (m_appInfo.stretchToFitEnabled()) {
			m_mainWindow->setView(sf::View(sf::FloatRect(0.0, 0.0, (float)m_appInfo.resx(), (float)m_appInfo.resy())));
		}
		else {
			int fullWidth = sf::VideoMode::getDesktopMode().width;
			int fullHeight = sf::VideoMode::getDesktopMode().height;
			int maxScaling = std::min(fullWidth / m_appInfo.resx(), fullHeight / m_appInfo.resy());
			int xOffset = (fullWidth - m_appInfo.resx()*maxScaling) / (2 * maxScaling);
			int yOffset = (fullHeight - m_appInfo.resy()*maxScaling) / (2 * maxScaling);
			m_mainWindow->setView(sf::View(sf::FloatRect((float)-xOffset, (float)-yOffset, (float)m_appInfo.resx() + 2 * xOffset, (float)m_appInfo.resy() + 2 * yOffset)));
		}
	}
	else {
		//Create Window
		m_mainWindow = new sf::RenderWindow(sf::VideoMode(m_appInfo.windowWidth(), m_appInfo.windowHeight()), "TBS_Game");

		//Handle stretching/scaling
		if (m_appInfo.stretchToFitEnabled()) {
			m_mainWindow->setView(sf::View(sf::FloatRect(0.0, 0.0, (float)m_appInfo.resx(), (float)m_appInfo.resy())));
		}
		else {
			int maxScaling = std::min(m_appInfo.windowWidth() / m_appInfo.resx(), m_appInfo.windowHeight() / m_appInfo.resy());
			int xOffset = (m_appInfo.windowWidth() - m_appInfo.resx()*maxScaling) / (2 * maxScaling);
			int yOffset = (m_appInfo.windowHeight() - m_appInfo.resy()*maxScaling) / (2 * maxScaling);
			m_mainWindow->setView(sf::View(sf::FloatRect((float)-xOffset, (float)-yOffset, (float)m_appInfo.resx() + 2 * xOffset, (float)m_appInfo.resy() + 2 * yOffset)));
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

AppInfo& Game::appInfo() {
	return m_appInfo;
}
