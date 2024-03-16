#include "pch.h"
#include "Page.h"

void updatePage(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database)
{
	switch (pageType)
	{
	case PageType::Home:
		homePageUpdate(gv, gw);
		break;
	case PageType::Authorization:
		authPageUpdate(gv, gw, database);
		break;
	case PageType::Registration:
		regPageUpdate(gv, gw, database);
		break;
	case PageType::GameMenu:
		menuUpdate(gv, gw, database);
		break;
	case PageType::MainMenu:
		menuUpdate(gv, gw, database);
		break;
	case PageType::HighScores:
		highScoresUpdate(gv, gw, database);
		break;
	case PageType::Settings:
		settingsUpdate(gv, gw);
		break;
	case PageType::GraphicsSettings:
		graphicsSettingsUpdate(gv, gw);
		break;
	case PageType::Profile:
		profileUpdate(gv, gw, database);
		break;
	case PageType::GameModeSelection:
		gameModeSelectionUpdate(gv, gw);
		break;
	}

	pageAction = PageAction::Nothing;
}

void openPage(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database)
{
	updatePage(gv, gw, database);

	while (gw->window.isOpen())
	{
		if (pageAction != PageAction::Nothing) { return; }

		sf::Event event;

		while (gw->window.pollEvent(event))
		{
			gw->pageGUI.handleEvent(event);
			if (event.type == sf::Event::Closed) { gw->window.close(); return; }
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape && pageType != PageType::MainMenu)
			{
				switch (pageType)
				{
				case PageType::Authorization:
					pageAction = PageAction::OpenHomePage;
					break;

				case PageType::Registration:
					pageAction = PageAction::OpenHomePage;
					break;

				case PageType::GraphicsSettings:
					pageAction = PageAction::OpenSettings;
					break;

				case PageType::Settings:
					if (gameState == GameState::GameMenu) { pageAction = PageAction::OpenGameMenu; }
					else if (gameState == GameState::MainMenu) { pageAction = PageAction::OpenMainMenu; }
					else if (gameState == GameState::GameResult) { pageAction = PageAction::OpenGameMenu; }
					break;

				case PageType::Profile:
					pageAction = PageAction::OpenMainMenu;
					break;

				case PageType::GameModeSelection:
					pageAction = PageAction::OpenMainMenu;
					break;

				case PageType::GameMenu:
					if (gameState != GameState::GameOver && gameState != GameState::GameResult)
					{
						pageAction = PageAction::ContinueGame;
					}
					break;

				case PageType::HighScores:
					if (gameState == GameState::GameMenu) { pageAction = PageAction::OpenGameMenu; }
					else if (gameState == GameState::MainMenu) { pageAction = PageAction::OpenMainMenu; }
					else if (gameState == GameState::GameResult) { pageAction = PageAction::OpenGameMenu; }
					break;
				}
			}
		}

		gw->window.clear(sf::Color::Black);
		gw->pageGUI.draw();
		gw->window.display();
	}
}

void pageEventHandler(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database, PageType _pageType)
{
	pageType = std::move(_pageType);
	openPage(gv, gw, database);

	while (gw->window.isOpen())
	{
		sf::Event event;
		while (gw->window.pollEvent(event)) { if (event.type == sf::Event::Closed) { gw->window.close(); return; } }
		gw->pageGUI.removeAllWidgets();

		switch (pageAction)
		{
		case PageAction::OpenHomePage:
			pageType = PageType::Home;
			openPage(gv, gw, database);
			break;

		case PageAction::OpenRegPage:
			pageType = PageType::Registration;
			openPage(gv, gw, database);
			break;

		case PageAction::OpenAuthPage:
			pageType = PageType::Authorization;
			openPage(gv, gw, database);
			break;

		case PageAction::OpenHighScores:
			pageType = PageType::HighScores;
			openPage(gv, gw, database);
			break;

		case PageAction::OpenProfile:
			pageType = PageType::Profile;
			openPage(gv, gw, database);
			break;

		case PageAction::OpenSettings:
			pageType = PageType::Settings;
			openPage(gv, gw, database);
			break;

		case PageAction::OpenGraphicsSettings:
			pageType = PageType::GraphicsSettings;
			openPage(gv, gw, database);
			break;

		case PageAction::OpenGameModeSelection:
			pageType = PageType::GameModeSelection;
			openPage(gv, gw, database);
			break;

		case PageAction::OpenMainMenu:
			pageType = PageType::MainMenu;
			openPage(gv, gw, database);
			break;

		case PageAction::OpenGameMenu:
			pageType = PageType::GameMenu;
			openPage(gv, gw, database);
			break;

		case PageAction::StartGame:
			gameState = GameState::StartGame;
			return;

		case PageAction::ContinueGame:
			gameState = GameState::ContinueGame;
			return;

		case PageAction::RestartGame:
			gameState = GameState::RestartGame;
			return;
		}
	}
}