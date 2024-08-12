#include "pch.h"
#include "Source.h"

void setVariable(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database)
{
	setConsole();

	sf::Vector2u windowSize(1920, 1080);
	bool isFullscreen = false;
	bool isVsync = true;
	unsigned int FPSLimiter = 75;

	gw->createWindow(windowSize, isFullscreen, isVsync, FPSLimiter);

	gv->consolasFont.loadFromFile("Fonts\\consolas.ttf");

	gv->setScore(0);
	gv->setGameTime(0);
	gv->setGameModeTime(0);
	gv->setMenuTime(0);
	gv->setNumOfMistakes(0);
	gv->setNumOfEnteredWords(0);
	gv->setSymbolNumber(0);

	gv->setPassword(L"");
	gv->setGameMode(GameMode::Free);

	gv->setGameLanguage(GameLanguage::English);

	fillDictionaryVec(gv);

	createVirtualKeyboard(gv, gw);

	for (size_t i = 0; i < 4; i++) { gv->personalHSVec.emplace_back(0); }

	gameState = GameState::MainMenu;
}

void resetVariable(std::unique_ptr<GameVariable>& gv, std::unique_ptr<Database>& database)
{
	gv->virtualKeyboardVec.clear();
	gv->wordsVec.clear();
}

int main()
{
	auto gv = std::make_unique<GameVariable>();
	auto gw = std::make_unique<GameWindow>();
	auto database = std::make_unique<Database>(gv);

	setVariable(gv, gw, database);

	if (dbError == DBError::SQLConnectError) { pageEventHandler(gv, gw, database, PageType::MainMenu); }
	else { pageEventHandler(gv, gw, database, PageType::Home); }

	if (gameState == GameState::StartGame) { startGame(gv, gw, database); }

	resetVariable(gv, database);

	return 1;
}