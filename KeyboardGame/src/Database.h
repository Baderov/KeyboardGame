#ifndef DATABASE_H
#define DATABASE_H

#include "GameVariable.h"

enum class DBAction
{
	GetUsers, GetConfig, GetHighScores
}; inline DBAction dbAction;

enum class DBError
{
	NoError, UserExists, SQLConnectError
}; inline DBError dbError;

enum class HighScores
{
	ThirtySeconds, OneMinute, TwoMinutes, OneMistake
}; inline HighScores highScores;

class Database
{
private:
	HENV hEnv;
	HDBC hDbc;
	HSTMT hStmt;
	SQLSMALLINT* iConnStrLength2Ptr;
	WCHAR* szConnStr;
	WCHAR szConnStrOut[256];
	SQLSMALLINT fieldCount, currentField;
	SQLWCHAR buf[512], colName[500];
	SQLLEN length;
	RETCODE rc;

	bool SQL_Connect();
	void SQL_Disconnect();
	SQLINTEGER DisplayError(SQLSMALLINT t, SQLHSTMT h);
	void SQL_FreeSTMT();
	SQLWCHAR* SQL_GetData(wchar_t* selectQuery);

public:
	Database(std::unique_ptr<GameVariable>& gv);
	~Database();

	bool executeInsertQuery(wchar_t* insertQuery);
	bool insertToUsersTable(std::unique_ptr<GameVariable>& gv);
	bool insertToConfigTable(std::unique_ptr<GameVariable>& gv);

	bool executeUpdateQuery(std::unique_ptr<GameVariable>& gv, wchar_t* updateQuery);
	bool updateConfigTable(std::unique_ptr<GameVariable>& gv);

	bool executeSelectQuery(std::unique_ptr<GameVariable>& gv, wchar_t* selectQuery);
	bool selectUsersTable(std::unique_ptr<GameVariable>& gv);
	bool selectConfigTable(std::unique_ptr<GameVariable>& gv, unsigned int userID);

	void getHS(std::unique_ptr<GameVariable>& gv, HighScores tempHS);

	unsigned int getUserID(std::unique_ptr<GameVariable>& gv, std::wstring nickname);
	SQLWCHAR* getUserNick(std::unique_ptr<GameVariable>& gv, unsigned int userID);

	bool userRegist(std::unique_ptr<GameVariable>& gv);
	bool userAuth(std::unique_ptr<GameVariable>& gv);
};

#endif