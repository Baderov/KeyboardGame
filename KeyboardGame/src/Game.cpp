#include "pch.h"
#include "Game.h"

const unsigned int DICTIONARY_SIZE = 2983;
const unsigned int SYMBOL_COST = 100;

void gameEventHandler(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database)
{
	sf::Event event;
	while (gw->window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			gw->window.close();
			break;

		case sf::Event::KeyPressed:
			gv->setPressedKey(sf::Keyboard::getDescription(event.key.scancode).toWideString());
			if (gameState != GameState::GameOver && gameState != GameState::GameResult && event.key.code != sf::Keyboard::Escape)
			{
				updateVirtualKeyboard(gv);
				updateWord(gv, gw);
			}
			gv->setPressedKey(L"");
			break;

		case sf::Event::KeyReleased:
			for (auto& virtualSymbol : gv->virtualKeyboardVec) { virtualSymbol->getRenderer()->setBackgroundColor(tgui::Color::White); }
			if (event.key.code == sf::Keyboard::Escape)
			{
				if (gameState != GameState::GameOver && gameState != GameState::GameResult) { gameState = GameState::GameMenu; }
				else if (gameState == GameState::GameOver || gameState == GameState::GameResult) { pageEventHandler(gv, gw, database, PageType::GameMenu); }
			}
			break;
		}
	}
}

void gameStateHandler(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database)
{
	switch (gameState)
	{
	case GameState::MainMenu:
		pageEventHandler(gv, gw, database, PageType::MainMenu);
		break;

	case GameState::GameMenu:
		pageEventHandler(gv, gw, database, PageType::GameMenu);
		gv->setMenuTime(gv->getGameClockElapsedTime() - (gv->getGameModeTime() - gv->getGameTime()));
		break;

	case GameState::GameOver:
		gameState = GameState::GameResult;
		createGameResult(gv, gw);
		if (dbError != DBError::SQLConnectError) { database->updateConfigTable(gv); }	
		break;

	case GameState::StartGame:
		resetVariable(gv, gw);
		updateWord(gv, gw);
		break;

	case GameState::RestartGame:
		resetVariable(gv, gw);
		updateWord(gv, gw);
		break;

	case GameState::ExitGame:
		gw->window.close();
		break;
	}
}

void startGame(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database)
{
	resetVariable(gv, gw);
	updateWord(gv, gw);

	while (gw->window.isOpen())
	{
		updateGame(gv, gw, database);
		drawGame(gw);
	}
}

void updateGame(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database)
{
	if (gameState == GameState::ContinueGame)
	{
		if (gv->getNumOfEnteredWords() + 1 == DICTIONARY_SIZE) { resetVariable(gv, gw); }
		if (gv->getGameMode() != GameMode::OneMistake && gv->getGameMode() != GameMode::Free) { updateGameTime(gv, gw); }
		if (gv->getGameMode() == GameMode::OneMistake && gv->getNumOfMistakes() >= 1) { gameState = GameState::GameOver; gv->setNumOfMistakes(0); }
	}

	gameEventHandler(gv, gw, database);
	gameStateHandler(gv, gw, database);

}

void drawGame(std::unique_ptr<GameWindow>& gw)
{
	gw->window.clear(sf::Color::Black);

	if (gameState == GameState::ContinueGame)
	{
		gw->virtualKeyboardGUI.draw();
		gw->wordsGUI.draw();
	}

	gw->gameInfoGUI.draw();

	gw->window.display();
}

void resetVariable(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw)
{
	gameState = GameState::ContinueGame;
	gameEvent = GameEvent::NewWord;

	gv->setScore(0);
	gv->setGameTime(0);
	gv->setMenuTime(0);
	gv->setNumOfMistakes(0);
	gv->setNumOfEnteredWords(0);
	gv->setSymbolNumber(0);
	gv->resetGameClock();

	if (gv->getGameMode() == GameMode::ThirtySeconds) { gv->setGameModeTime(30); }
	else if (gv->getGameMode() == GameMode::OneMinute) { gv->setGameModeTime(60); }
	else if (gv->getGameMode() == GameMode::TwoMinutes) { gv->setGameModeTime(120); }
	else if (gv->getGameMode() == GameMode::Free) { gv->setGameModeTime(0); }
	else if (gv->getGameMode() == GameMode::OneMistake) { gv->setGameModeTime(0); }

	shuffleDictionaryVec(gv);
	createGameInfo(gv, gw);

}

void updateGameTime(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw)
{
	if (gv->getGameMode() != GameMode::OneMistake && gv->getGameMode() != GameMode::Free && gameState != GameState::GameOver && gameState != GameState::GameResult)
	{
		tgui::Label::Ptr timeLabel = gw->gameInfoGUI.get<tgui::Label>("timeLabel");
		gv->setGameTime((gv->getGameModeTime() - gv->getGameClockElapsedTime()) + gv->getMenuTime());
		if (gv->getGameLanguage() == GameLanguage::English) { timeLabel->setText(L"Time: " + std::to_wstring(gv->getGameTime())); }
		else if (gv->getGameLanguage() == GameLanguage::Russian) { timeLabel->setText(L"¬рем€: " + std::to_wstring(gv->getGameTime())); }

		if (gv->getGameTime() <= 0) { gameState = GameState::GameOver; }
	}
}

void updateVirtualKeyboard(std::unique_ptr<GameVariable>& gv)
{
	for (auto& virtualSymbol : gv->virtualKeyboardVec)
	{
		if (gv->getPressedKey() == virtualSymbol->getText())
		{
			virtualSymbol->getRenderer()->setBackgroundColor(tgui::Color::Green);
			break;
		}
	}
}

void updateWord(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw)
{
	tgui::Label::Ptr correctlyLabel = gw->gameInfoGUI.get<tgui::Label>("correctlyLabel");
	tgui::Label::Ptr wrongLabel = gw->gameInfoGUI.get<tgui::Label>("wrongLabel");

	if (gameEvent == GameEvent::NoEvent)
	{
		if (gv->getPressedKey() == gv->wordsVec[gv->getSymbolNumber()]->getText())
		{
			gv->wordsVec[gv->getSymbolNumber()]->getRenderer()->setBackgroundColor(tgui::Color::Green);
			gv->setSymbolNumber(gv->getSymbolNumber() + 1);
			gv->setScore(gv->getScore() + SYMBOL_COST);
			correctlyLabel->setVisible(true);
			wrongLabel->setVisible(false);
		}
		else if (gv->getPressedKey() != gv->wordsVec[gv->getSymbolNumber()]->getText() && gv->getPressedKey() != L"Esc")
		{
			gv->wordsVec[gv->getSymbolNumber()]->getRenderer()->setBackgroundColor(tgui::Color::Red);
			gv->setNumOfMistakes(gv->getNumOfMistakes() + 1);
			if (gv->getScore() > 0 && gv->getGameMode() != GameMode::OneMistake) { gv->setScore(gv->getScore() - SYMBOL_COST); }

			correctlyLabel->setVisible(false);
			wrongLabel->setVisible(true);
		}

		if (gv->getSymbolNumber() == gv->wordsVec.size())
		{
			gv->setNumOfEnteredWords(gv->getNumOfEnteredWords() + 1);
			gv->setSymbolNumber(0);
			gameEvent = GameEvent::NewWord;
		}

		if (gv->getGameMode() != GameMode::Free)
		{
			tgui::Label::Ptr scoreLabel = gw->gameInfoGUI.get<tgui::Label>("scoreLabel");
			tgui::Label::Ptr enteredWordsLabel = gw->gameInfoGUI.get<tgui::Label>("enteredWordsLabel");
			tgui::Label::Ptr mistakesLabel = gw->gameInfoGUI.get<tgui::Label>("mistakesLabel");

			if (gv->getGameLanguage() == GameLanguage::English)
			{
				scoreLabel->setText(L"Score: " + std::to_wstring(gv->getScore()));
				enteredWordsLabel->setText(L"Entered words: " + std::to_wstring(gv->getNumOfEnteredWords()));
				if (gv->getGameMode() != GameMode::OneMistake) { mistakesLabel->setText(L"Mistakes: " + std::to_wstring(gv->getNumOfMistakes())); }
			}
			else if (gv->getGameLanguage() == GameLanguage::Russian)
			{
				scoreLabel->setText(L"—чЄт: " + std::to_wstring(gv->getScore()));
				enteredWordsLabel->setText(L"¬ведЄнные слова: " + std::to_wstring(gv->getNumOfEnteredWords()));
				if (gv->getGameMode() != GameMode::OneMistake) { mistakesLabel->setText(L"ќшибки: " + std::to_wstring(gv->getNumOfMistakes())); }
			}
		}
	}

	if (gameEvent == GameEvent::NewWord)
	{
		createWord(gv, gw);
		correctlyLabel->setVisible(false);
		wrongLabel->setVisible(false);
		gameEvent = GameEvent::NoEvent;
	}
}