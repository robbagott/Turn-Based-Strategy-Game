#include "Game.h"
#include "SplashScreen.h"
#include "MainMenu.h"
#include "NullGameState.h"

bool Game::m_isInstantiated = false;

Game::Game() {
	//There can only be one
	assert(!Game::m_isInstantiated);
	Game::m_isInstantiated = true;
	Game::m_quitRequested = false;
	Game::m_changeStateRequested = false;

	m_appInfo = new AppInfo("config.txt");
	m_gameState = new NullGameState();
	m_nextGameState = new NullGameState();
}

Game::~Game() {
	delete m_appInfo;
	delete m_mainWindow;
	delete m_gameState;
	delete m_nextGameState;
}

void Game::init() {
	initializeWindow();
	m_mainWindow->clear(sf::Color::Black);
	m_mainWindow->display();

	if (m_appInfo->splashEnabled()) {
		IGameState* newState = new SplashScreen("../Assets/Graphics/SplashScreen.bmp");
		m_gameState = newState;
		newState->init();
	}
	else {
		IGameState* newState = new MainMenu();
		m_gameState = newState;
		newState->init();
	}
}

//Outside code calls this
void Game::start() {

	init();

	while (!m_quitRequested) {
		gameLoop();
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
	if (m_appInfo->screenMode() == AppInfo::SM_FULLSCREEN) {
		//Create window
		m_mainWindow = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "TBS_Game", sf::Style::Fullscreen);

		//Handle stretching/scaline
		if (m_appInfo->stretchToFitEnabled()) {
			m_mainWindow->setView(sf::View(sf::FloatRect(0.0, 0.0, (float)m_appInfo->resx(), (float)m_appInfo->resy())));
		}
		else {
			int fullWidth = sf::VideoMode::getDesktopMode().width;
			int fullHeight = sf::VideoMode::getDesktopMode().height;
			int maxScaling = std::min(fullWidth / m_appInfo->resx(), fullHeight / m_appInfo->resy());
			int xOffset = (fullWidth - m_appInfo->resx()*maxScaling) / (2 * maxScaling);
			int yOffset = (fullHeight - m_appInfo->resy()*maxScaling) / (2 * maxScaling);
			m_mainWindow->setView(sf::View(sf::FloatRect((float)-xOffset, (float)-yOffset, (float)m_appInfo->resx() + 2 * xOffset, (float)m_appInfo->resy() + 2 * yOffset)));
		}
	}
	else if (m_appInfo->screenMode() == AppInfo::SM_BORDERLESSWINDOW) {
		//Create window
		m_mainWindow = new sf::RenderWindow(sf::VideoMode(m_appInfo->windowWidth(), m_appInfo->windowHeight()), "TBS_Game", sf::Style::None);

		//Handle stretching/scaling
		if (m_appInfo->stretchToFitEnabled()) {
			m_mainWindow->setView(sf::View(sf::FloatRect(0.0, 0.0, (float)m_appInfo->resx(), (float)m_appInfo->resy())));
		}
		else {
			int fullWidth = sf::VideoMode::getDesktopMode().width;
			int fullHeight = sf::VideoMode::getDesktopMode().height;
			int maxScaling = std::min(fullWidth / m_appInfo->resx(), fullHeight / m_appInfo->resy());
			int xOffset = (fullWidth - m_appInfo->resx()*maxScaling) / (2 * maxScaling);
			int yOffset = (fullHeight - m_appInfo->resy()*maxScaling) / (2 * maxScaling);
			m_mainWindow->setView(sf::View(sf::FloatRect((float)-xOffset, (float)-yOffset, (float)m_appInfo->resx() + 2 * xOffset, (float)m_appInfo->resy() + 2 * yOffset)));
		}
	}
	else {
		//Create Window
		m_mainWindow = new sf::RenderWindow(sf::VideoMode(m_appInfo->windowWidth(), m_appInfo->windowHeight()), "TBS_Game");

		//Handle stretching/scaling
		if (m_appInfo->stretchToFitEnabled()) {
			m_mainWindow->setView(sf::View(sf::FloatRect(0.0, 0.0, (float)m_appInfo->resx(), (float)m_appInfo->resy())));
		}
		else {
			int maxScaling = std::min(m_appInfo->windowWidth() / m_appInfo->resx(), m_appInfo->windowHeight() / m_appInfo->resy());
			int xOffset = (m_appInfo->windowWidth() - m_appInfo->resx()*maxScaling) / (2 * maxScaling);
			int yOffset = (m_appInfo->windowHeight() - m_appInfo->resy()*maxScaling) / (2 * maxScaling);
			m_mainWindow->setView(sf::View(sf::FloatRect((float)-xOffset, (float)-yOffset, (float)m_appInfo->resx() + 2 * xOffset, (float)m_appInfo->resy() + 2 * yOffset)));
		}
	}
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

void Game::swapState() {
	assert(m_changeStateRequested);

	m_gameState->cleanup();
	delete m_gameState;
	m_gameState = m_nextGameState;

	//Reset nextGameState to NullGameState. 
	//We don't want to accidentally deallocate the current gameState by deallocating nextGameState
	m_nextGameState = new NullGameState();
	m_gameState->init();

	m_changeStateRequested = false;
}

void Game::handleEvents() {
	m_gameState->handleEvents(*this);
}

void Game::update() {
	m_gameState->update(*this);
}

void Game::draw() {
	m_gameState->draw(*this);
}