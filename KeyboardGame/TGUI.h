#ifndef TGUI_H
#define TGUI_H

#include "GameVariable.h"
#include "GameWindow.h"
#include "Page.h"
#include "Database.h"
#include "Source.h"

enum class WidgetState
{
	Nothing, ResolutionComboBoxChanged, LanguageComboBoxChanged, FPSComboBoxChanged, FullscreenCheckBoxChanged, VsyncCheckBoxChanged
}; inline WidgetState widgetState;

void widgetStateHandler(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw);
void menuApplyButtonPressed(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw);

void homePageUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw);
void regPageUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database);
void authPageUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database);
void graphicsSettingsUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw);
void settingsUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw);
void highScoresUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database);
void menuUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database);
void profileUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database);
void gameModeSelectionUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw);

void createGameInfo(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw);
void createVirtualKeyboard(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw);
void createWord(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw);
void createGameResult(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw);

#endif