#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

class GameWindow
{
private:
	tgui::Font consolasFont;
	sf::ContextSettings settings;
	sf::Vector2u size;
	unsigned int FPSLimiter;
	bool isFullscreen;
	bool isVsync;
public:
	sf::RenderWindow window;
	tgui::Gui pageGUI;
	tgui::Gui wordsGUI;
	tgui::Gui virtualKeyboardGUI;
	tgui::Gui gameInfoGUI;

	void createWindow(sf::Vector2u tempSize, bool tempIsFullscreen, bool tempIsVsync, unsigned int tempFPS);

	void setSize(sf::Vector2u tempSize);
	void setFPSLimiter(unsigned int tempFPSLimiter);
	void setIsFullscreen(bool tempIsFullscreen);
	void setIsVsync(bool tempIsVsync);

	sf::Vector2u getSize();
	unsigned int getFPSLimiter();
	bool getIsFullscreen();
	bool getIsVsync();
};

#endif