#include "pch.h"
#include "GameWindow.h"

void GameWindow::createWindow(sf::Vector2u tempSize, bool tempIsFullscreen, bool tempIsVsync, unsigned int tempFPS)
{
	settings.antialiasingLevel = 8;

	size = std::move(tempSize);
	isFullscreen = std::move(tempIsFullscreen);
	isVsync = std::move(tempIsVsync);
	FPSLimiter = std::move(tempFPS);

	if (isFullscreen) { window.create(sf::VideoMode(size.x, size.y), "Keyboard Game", sf::Style::Fullscreen, settings); }
	else { window.create(sf::VideoMode(size.x, size.y), "Keyboard Game", sf::Style::Close, settings); }

	if (isVsync) { window.setVerticalSyncEnabled(true); window.setFramerateLimit(0); }
	else { window.setVerticalSyncEnabled(false); window.setFramerateLimit(FPSLimiter); }
	window.setKeyRepeatEnabled(false);

	pageGUI.setWindow(window);
	wordsGUI.setWindow(window);
	virtualKeyboardGUI.setWindow(window);
	gameInfoGUI.setWindow(window);

	consolasFont.setGlobalFont("Fonts\\consolas.ttf");

	pageGUI.setFont(consolasFont);
	wordsGUI.setFont(consolasFont);
	virtualKeyboardGUI.setFont(consolasFont);
	gameInfoGUI.setFont(consolasFont);
}

sf::Vector2u GameWindow::getSize()
{
	sf::Vector2u tempSize = size;
	return tempSize;
}

unsigned int GameWindow::getFPSLimiter()
{
	unsigned int tempFPSLimiter = FPSLimiter;
	return tempFPSLimiter;
}

bool GameWindow::getIsFullscreen()
{
	bool tempIsFullscreen = isFullscreen;
	return tempIsFullscreen;
}
bool GameWindow::getIsVsync()
{
	bool tempIsVsync = isVsync;
	return tempIsVsync;
}

void GameWindow::setSize(sf::Vector2u tempSize) { size = std::move(tempSize); }
void GameWindow::setFPSLimiter(unsigned int tempFPSLimiter) { FPSLimiter = std::move(tempFPSLimiter); }
void GameWindow::setIsFullscreen(bool tempIsFullscreen) { isFullscreen = std::move(tempIsFullscreen); }
void GameWindow::setIsVsync(bool tempIsVsync) { isVsync = std::move(tempIsVsync); }