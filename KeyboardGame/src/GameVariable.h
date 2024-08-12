#ifndef GAME_VARIABLE_H
#define GAME_VARIABLE_H

enum class GameMode
{
	ThirtySeconds, OneMinute, TwoMinutes, OneMistake, Free
};

enum class GameLanguage
{
	English, Russian
};

class GameVariable
{
private:
	sf::Clock gameClock;
	std::wstring nickname;
	std::wstring password;
	std::wstring pressedKey;
	GameMode gameMode;
	GameLanguage gameLanguage;
	unsigned int symbolNumber;
	unsigned int numOfEnteredWords;
	unsigned int numOfMistakes;
	unsigned int score;
	unsigned int gameTime;
	unsigned int menuTime;
	unsigned int gameModeTime;
	unsigned int avatarNumber;
	bool showDBConnectionError;
public:
	const std::string symbols = "QWERTYUIOPASDFGHJKLZXCVBNM";

	sf::Font consolasFont;

	std::vector<tgui::Button::Ptr> virtualKeyboardVec;
	std::vector<tgui::Button::Ptr> wordsVec;
	std::vector<std::wstring> dictionaryVector;
	std::vector<unsigned int> personalHSVec; // 4 personal scores: 1 - 30 seconds, 2 - 60 seconds,  3 - 120 seconds, 4 - mistake mode.
	std::vector<std::pair<std::wstring, unsigned int>> HSVec; // nickname and score.
	std::vector<std::pair<std::wstring, std::wstring>> usersVec; // nickname and password.

	std::wstring getNickname();
	std::wstring getPassword();
	std::wstring getPressedKey();
	GameMode getGameMode();
	GameLanguage getGameLanguage();
	unsigned int getSymbolNumber();
	unsigned int getNumOfEnteredWords();
	unsigned int getNumOfMistakes();
	unsigned int getScore();
	unsigned int getGameTime();
	unsigned int getMenuTime();
	unsigned int getGameModeTime();
	unsigned int getAvatarNumber();
	unsigned int getGameClockElapsedTime();
	bool getShowDBConnectionError();

	void resetGameClock();
	void setNickname(std::wstring tempNickname);
	void setPassword(std::wstring tempPassword);
	void setPressedKey(std::wstring tempPressedKey);
	void setGameMode(GameMode tempGameMode);
	void setGameLanguage(GameLanguage tempGameLanguage);
	void setSymbolNumber(unsigned int tempSymbolNumber);
	void setNumOfEnteredWords(unsigned int tempNumOfEnteredWords);
	void setNumOfMistakes(unsigned int tempNumOfMistakes);
	void setScore(unsigned int tempScore);
	void setGameTime(unsigned int tempGameTime);
	void setMenuTime(unsigned int tempMenuTime);
	void setGameModeTime(unsigned int tempGameModeTime);
	void setAvatarNumber(unsigned int tempAvatarNumber);
	void setShowDBConnectionError(bool tempShowError);
};

#endif