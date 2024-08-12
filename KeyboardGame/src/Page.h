#ifndef PAGES_H
#define PAGES_H

#include "GameVariable.h"
#include "Game.h"
#include "Database.h"
#include "TGUI.h"

enum class PageType
{
	Home, Authorization, Registration, GameMenu, MainMenu, Settings, GraphicsSettings, HighScores, Profile, GameModeSelection
}; inline PageType pageType;

enum class PageAction
{
	Nothing, OpenHomePage, OpenAuthPage, OpenRegPage, OpenGameMenu, OpenMainMenu, OpenSettings, OpenGraphicsSettings, OpenHighScores, OpenProfile, OpenGameModeSelection, StartGame, RestartGame, ContinueGame
}; inline PageAction pageAction;

void updatePage(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database);
void openPage(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database);
void pageEventHandler(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database, PageType _pageType);

#endif