#ifndef GAME_H
#define GAME_H

#include "GameVariable.h"
#include "GameWindow.h"
#include "TGUI.h"
#include "Database.h"
#include "File.h"

enum class GameState
{
	GameMenu, MainMenu, StartGame, ContinueGame, GameOver, GameResult, RestartGame, ExitGame
}; inline GameState gameState;

enum class GameEvent
{
	NoEvent, NewWord
}; inline GameEvent gameEvent;


void gameStateHandler(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database);
void gameEventHandler(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database);
void startGame(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database);
void updateGame(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database);
void drawGame(std::unique_ptr<GameWindow>& gw);
void resetVariable(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw);


void updateGameTime(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw);
void updateWord(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw);
void updateVirtualKeyboard(std::unique_ptr<GameVariable>& gv);

#endif
