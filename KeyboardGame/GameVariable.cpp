#include "pch.h"
#include "GameVariable.h"

// GETTERS.
std::wstring GameVariable::getNickname()
{
	std::wstring tempNickname = nickname;
	return tempNickname;
}

std::wstring GameVariable::getPassword()
{
	std::wstring tempPassword = password;
	return tempPassword;
}

std::wstring GameVariable::getPressedKey()
{
	std::wstring tempPressedKey = pressedKey;
	return tempPressedKey;
}

GameMode GameVariable::getGameMode()
{
	GameMode tempGameMode = gameMode;
	return tempGameMode;
}

unsigned int GameVariable::getSymbolNumber()
{
	unsigned int tempSymbolNumber = symbolNumber;
	return tempSymbolNumber;
}

unsigned int GameVariable::getNumOfEnteredWords()
{
	unsigned int tempNumOfEnteredWords = numOfEnteredWords;
	return tempNumOfEnteredWords;
}

unsigned int GameVariable::getNumOfMistakes()
{
	unsigned int tempNumOfMistakes = numOfMistakes;
	return tempNumOfMistakes;
}

unsigned int GameVariable::getScore()
{
	unsigned int tempScore = score;
	return tempScore;
}

unsigned int GameVariable::getGameTime()
{
	unsigned int tempGameTime = gameTime;
	return tempGameTime;
}

unsigned int GameVariable::getMenuTime()
{
	unsigned int tempMenuTime = menuTime;
	return tempMenuTime;
}

unsigned int GameVariable::getGameModeTime()
{
	unsigned int tempGameModeTime = gameModeTime;
	return tempGameModeTime;
}

unsigned int GameVariable::getAvatarNumber()
{
	unsigned int tempAvatarNumber = avatarNumber;
	return tempAvatarNumber;
}

unsigned int GameVariable::getGameClockElapsedTime()
{
	unsigned int elapsedTime = static_cast<unsigned int>(gameClock.getElapsedTime().asSeconds());
	return elapsedTime;
}

GameLanguage GameVariable::getGameLanguage()
{
	GameLanguage tempGameLanguage = gameLanguage;
	return tempGameLanguage;
}

bool GameVariable::getShowDBConnectionError()
{
	bool tempShowDBConnectionError = showDBConnectionError;
	return tempShowDBConnectionError;
}

// SETTERS.
void GameVariable::resetGameClock() { gameClock.restart(); }
void GameVariable::setNickname(std::wstring tempNickname) { nickname = std::move(tempNickname); }
void GameVariable::setPassword(std::wstring tempPassword) { password = std::move(tempPassword); }
void GameVariable::setPressedKey(std::wstring tempPressedKey) { pressedKey = std::move(tempPressedKey); }
void GameVariable::setGameMode(GameMode tempGameMode) { gameMode = std::move(tempGameMode); }
void GameVariable::setSymbolNumber(unsigned int tempSymbolNumber) { symbolNumber = std::move(tempSymbolNumber); }
void GameVariable::setNumOfEnteredWords(unsigned int tempNumOfEnteredWords) { numOfEnteredWords = std::move(tempNumOfEnteredWords); }
void GameVariable::setNumOfMistakes(unsigned int tempNumOfMistakes) { numOfMistakes = std::move(tempNumOfMistakes); }
void GameVariable::setScore(unsigned int tempScore) { score = std::move(tempScore); }
void GameVariable::setGameTime(unsigned int tempGameTime) { gameTime = std::move(tempGameTime); }
void GameVariable::setMenuTime(unsigned int tempMenuTime) { menuTime = std::move(tempMenuTime); }
void GameVariable::setGameModeTime(unsigned int tempGameModeTime) { gameModeTime = std::move(tempGameModeTime); }
void GameVariable::setAvatarNumber(unsigned int tempAvatarNumber) { avatarNumber = std::move(tempAvatarNumber); }
void GameVariable::setGameLanguage(GameLanguage tempGameLanguage) { gameLanguage = std::move(tempGameLanguage); }
void GameVariable::setShowDBConnectionError(bool tempShowDBConnectionError) { showDBConnectionError = std::move(tempShowDBConnectionError); }
