#include "pch.h"
#include "Database.h"

Database::Database(std::unique_ptr<GameVariable>& gv)
{
	hEnv = NULL;
	hDbc = NULL;
	hStmt = NULL;
	iConnStrLength2Ptr = nullptr;
	szConnStr = nullptr;
	fieldCount = 0, currentField = 0;
	length = 0;
	rc = 0;

	if (!SQL_Connect())
	{
		dbError = DBError::SQLConnectError;
		gv->setShowDBConnectionError(true);
		DEBUG_MSG(L"SQL CONNECT ERROR");
	}
}

Database::~Database() { SQL_Disconnect(); }

SQLINTEGER Database::DisplayError(SQLSMALLINT t, SQLHSTMT h)
{
	SQLWCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER nativeError;
	SQLSMALLINT i, MsgLen;
	SQLRETURN rc;

	SQLLEN numRecs = 0;
	SQLGetDiagField(t, h, 0, SQL_DIAG_NUMBER, &numRecs, 0, 0);

	// Get the status records.
	i = 1;

	while (i <= numRecs && (rc = SQLGetDiagRec(t, h, i, SqlState, &nativeError, Msg, sizeof(Msg), &MsgLen)) != SQL_NO_DATA)
	{
		wprintf(L"Error %ld: %s\n", nativeError, Msg);
		i++;
	}

	return nativeError;
}

bool Database::SQL_Connect()
{
	SQLSMALLINT CONN_STR_OUT_MAX = 255;
	std::wstring tempConnString = L"Driver={SQL Server}; Server=.\\SQLExpress; AttachDbFilename=" + std::filesystem::current_path().wstring() + L"\\Database\\GameDB.mdf; Database=GameDB; Trusted_Connection=Yes;";
	wchar_t* connString = const_cast<wchar_t*>(tempConnString.c_str());

	rc = SQLAllocEnv(&hEnv); /* Allocate an environment handle */
	rc = SQLAllocConnect(hEnv, &hDbc); 	/* Allocate a connection handle */
	rc = SQLDriverConnect(hDbc, NULL, connString, SQL_NTS, szConnStrOut, CONN_STR_OUT_MAX, iConnStrLength2Ptr, SQL_DRIVER_NOPROMPT); /* Connect to the database */

	if (SQL_SUCCEEDED(rc))
	{
		DEBUG_MSG(L"Successful connection to the database!");
		return true;
	}
	else
	{
		SQLINTEGER DBAlreadyExistsError = 1801;

		if (DisplayError(SQL_HANDLE_DBC, hDbc) == DBAlreadyExistsError)
		{
			wchar_t connString[200] = L"Driver={SQL Server}; Server=.\\SQLExpress; Database=GameDB; Trusted_Connection=Yes;";
			rc = SQLDriverConnect(hDbc, NULL, connString, SQL_NTS, szConnStrOut, CONN_STR_OUT_MAX, iConnStrLength2Ptr, SQL_DRIVER_NOPROMPT); /* Connect to the database */
			if (SQL_SUCCEEDED(rc))
			{
				DEBUG_MSG(L"Successful connection to the database!");
				return true;
			}
		}

		DEBUG_MSG(L"Database connection failed!");
		return false;
	}
}

void Database::SQL_Disconnect()
{
	/* Disconnect and free up allocated handles */
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	SQLDisconnect(hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
	if (dbError != DBError::SQLConnectError) { DEBUG_MSG(L"Successful disconnect from the database"); }
}

void Database::SQL_FreeSTMT()
{
	SQLFreeStmt(hStmt, SQL_UNBIND);
	SQLFreeStmt(hStmt, SQL_CLOSE);
}

SQLWCHAR* Database::SQL_GetData(wchar_t* selectQuery)
{
	rc = SQLPrepare(hStmt, selectQuery, SQL_NTS);

	/* Excecute the query */
	rc = SQLExecute(hStmt);
	if (!SQL_SUCCEEDED(rc))
	{
		DEBUG_MSG(L"SQL Failed");
		DisplayError(SQL_HANDLE_STMT, hStmt);
		return nullptr;
	}

	SQLNumResultCols(hStmt, &fieldCount);

	if (fieldCount <= 0)
	{
		DEBUG_MSG(L"Error: Number of fields in the result set is 0");
		return nullptr;
	}

	for (currentField = 1; currentField <= fieldCount; currentField++) { SQLDescribeCol(hStmt, currentField, colName, sizeof(colName), 0, 0, 0, 0, 0); }

	rc = SQLFetch(hStmt);

	while (SQL_SUCCEEDED(rc))
	{
		for (currentField = 1; currentField <= fieldCount; currentField++)
		{
			rc = SQLGetData(hStmt, currentField, SQL_C_WCHAR, buf, sizeof(buf), &length);

			if (SQL_SUCCEEDED(rc) == FALSE)
			{
				DEBUG_MSG(static_cast<int>(currentField) << L": SQLGetData failed");
				continue;
			}

			if (length <= 0)
			{
				DEBUG_MSG(static_cast<int>(currentField) << L": (no data)");
				continue;
			}

			SQL_FreeSTMT();

			return buf;
		}
		rc = SQLFetch(hStmt);
	};
}

bool Database::executeInsertQuery(wchar_t* insertQuery)
{
	rc = SQLPrepare(hStmt, insertQuery, SQL_NTS);
	rc = SQLExecute(hStmt);

	if (SQL_SUCCEEDED(rc))
	{
		DEBUG_MSG(L"Data inserted successfully!");
		SQL_FreeSTMT();
		return true;
	}
	else
	{
		DEBUG_MSG(L"Insert data error!");
		DisplayError(SQL_HANDLE_STMT, hStmt);
		SQL_FreeSTMT();
		return false;
	}
}

bool Database::insertToUsersTable(std::unique_ptr<GameVariable>& gv)
{
	wchar_t query[] = L"INSERT INTO Users (Nickname, Password) VALUES (?, ?)";
	std::wstring nickname = gv->getNickname();
	std::wstring password = gv->getPassword();
	rc = SQLAllocStmt(hDbc, &hStmt);

	SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_VARCHAR, std::wcslen(nickname.c_str()), 0, const_cast<wchar_t*>(nickname.c_str()), 0, 0);
	SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_VARCHAR, std::wcslen(password.c_str()), 0, const_cast<wchar_t*>(password.c_str()), 0, 0);

	if (executeInsertQuery(query)) { return true; }
	else { return false; }
}

bool Database::insertToConfigTable(std::unique_ptr<GameVariable>& gv)
{
	wchar_t query[] = L"INSERT INTO Config (UserID, PHS_30, PHS_60, PHS_120, PHS_Mistake, AvatarNumber) VALUES (?, ?, ?, ?, ?, ?)";
	unsigned int userID = getUserID(gv, gv->getNickname());
	unsigned int avatarNumber = gv->getAvatarNumber();
	rc = SQLAllocStmt(hDbc, &hStmt);

	SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, std::to_string(userID).length(), 0, &userID, 0, 0);
	SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, std::to_string(gv->personalHSVec[0]).length(), 0, &gv->personalHSVec[0], 0, 0);
	SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, std::to_string(gv->personalHSVec[1]).length(), 0, &gv->personalHSVec[1], 0, 0);
	SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, std::to_string(gv->personalHSVec[2]).length(), 0, &gv->personalHSVec[2], 0, 0);
	SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, std::to_string(gv->personalHSVec[3]).length(), 0, &gv->personalHSVec[3], 0, 0);
	SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, std::to_string(avatarNumber).length(), 0, &avatarNumber, 0, 0);

	if (executeInsertQuery(query)) { return true; }
	else { return false; }
}

bool Database::executeUpdateQuery(std::unique_ptr<GameVariable>& gv, wchar_t* updateQuery)
{
	rc = SQLPrepare(hStmt, updateQuery, SQL_NTS);
	rc = SQLExecute(hStmt);

	if (SQL_SUCCEEDED(rc)) { DEBUG_MSG(L"Data updated successfully!"); SQL_FreeSTMT(); return true; }
	else
	{
		DEBUG_MSG(L"Update data error!");
		DisplayError(SQL_HANDLE_STMT, hStmt);
		SQL_FreeSTMT();
		return false;
	}
}

bool Database::updateConfigTable(std::unique_ptr<GameVariable>& gv)
{
	wchar_t* query = const_cast<wchar_t*>(L"");
	unsigned int score = gv->getScore();
	unsigned int userID = getUserID(gv, gv->getNickname());

	selectConfigTable(gv, userID);

	switch (gv->getGameMode())
	{
	case GameMode::ThirtySeconds:
		if (gv->getScore() > gv->personalHSVec[0])
		{
			query = const_cast<wchar_t*>(L"UPDATE Config SET PHS_30 = ? WHERE UserID = ?");
		}
		break;

	case GameMode::OneMinute:
		if (gv->getScore() > gv->personalHSVec[1])
		{
			query = const_cast<wchar_t*>(L"UPDATE Config SET PHS_60 = ? WHERE UserID = ?");
		}
		break;

	case GameMode::TwoMinutes:
		if (gv->getScore() > gv->personalHSVec[2])
		{
			query = const_cast<wchar_t*>(L"UPDATE Config SET PHS_120 = ? WHERE UserID = ?");
		}
		break;

	case GameMode::OneMistake:
		if (gv->getScore() > gv->personalHSVec[3])
		{
			query = const_cast<wchar_t*>(L"UPDATE Config SET PHS_Mistake = ? WHERE UserID = ?");
		}
		break;
	}

	if (query != L"")
	{
		rc = SQLAllocStmt(hDbc, &hStmt);
		SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, std::to_string(gv->getScore()).length(), 0, &score, 0, 0);
		SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, std::to_string(userID).length(), 0, &userID, 0, 0);

		if (executeUpdateQuery(gv, query)) { return true; }
		else { return false; }
	}
	else { return false; }
}

bool Database::executeSelectQuery(std::unique_ptr<GameVariable>& gv, wchar_t* selectQuery)
{
	rc = SQLPrepare(hStmt, selectQuery, SQL_NTS);
	rc = SQLExecute(hStmt);

	if (!SQL_SUCCEEDED(rc))
	{
		DEBUG_MSG(L"SQL Failed");
		DisplayError(SQL_HANDLE_STMT, hStmt);
		SQL_FreeSTMT();
		return false;
	}

	SQLNumResultCols(hStmt, &fieldCount);

	if (fieldCount <= 0)
	{
		DEBUG_MSG(L"Error: Number of fields in the result set is 0");
		SQL_FreeSTMT();
		return false;
	}

	for (currentField = 1; currentField <= fieldCount; currentField++)
	{
		SQLDescribeCol(hStmt, currentField, colName, sizeof(colName), 0, 0, 0, 0, 0);
	}

	rc = SQLFetch(hStmt);

	while (SQL_SUCCEEDED(rc))
	{
		for (currentField = 1; currentField <= fieldCount; currentField++)
		{
			rc = SQLGetData(hStmt, currentField, SQL_C_WCHAR, buf, sizeof(buf), &length);

			if (SQL_SUCCEEDED(rc) == FALSE)
			{
				DEBUG_MSG(static_cast<int>(currentField) << L": SQLGetData failed");
				continue;
			}
			if (length <= 0)
			{
				DEBUG_MSG(static_cast<int>(currentField) << L": (no data)");
				continue;
			}

			switch (dbAction)
			{
			case DBAction::GetUsers:
				if (currentField == 1) { gv->usersVec.emplace_back(std::make_pair(buf, L"")); }
				else if (currentField == 2) { gv->usersVec.back().second = buf; }
				break;

			case DBAction::GetConfig:
				if (currentField == 1) { gv->personalHSVec.emplace_back(_wtoi(buf)); }
				else if (currentField == 2) { gv->personalHSVec.emplace_back(_wtoi(buf)); }
				else if (currentField == 3) { gv->personalHSVec.emplace_back(_wtoi(buf)); }
				else if (currentField == 4) { gv->personalHSVec.emplace_back(_wtoi(buf)); }
				else if (currentField == 5) { gv->setAvatarNumber(_wtoi(buf)); }
				break;

			case DBAction::GetHighScores:
				if (currentField == 1) { gv->HSVec.emplace_back(std::make_pair(buf, 0)); }
				else if (currentField == 2) { gv->HSVec.back().second = _wtoi(buf); }
				break;
			}
		}
		rc = SQLFetch(hStmt);
	};

	SQL_FreeSTMT();
	return true;
}

bool Database::selectUsersTable(std::unique_ptr<GameVariable>& gv)
{
	dbAction = DBAction::GetUsers;

	gv->usersVec.clear();

	wchar_t query[] = L"SELECT Nickname, Password FROM Users";

	rc = SQLAllocStmt(hDbc, &hStmt);

	if (executeSelectQuery(gv, query)) { return true; }
	else { return false; }
}

bool Database::selectConfigTable(std::unique_ptr<GameVariable>& gv, unsigned int userID)
{
	dbAction = DBAction::GetConfig;

	gv->personalHSVec.clear();

	wchar_t query[] = L"SELECT PHS_30, PHS_60, PHS_120, PHS_Mistake, AvatarNumber FROM Config WHERE UserID = ?";

	rc = SQLAllocStmt(hDbc, &hStmt);

	SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, std::to_string(userID).length(), 0, &userID, 0, 0);

	if (executeSelectQuery(gv, query)) { return true; }
	else { return false; }
}

void Database::getHS(std::unique_ptr<GameVariable>& gv, HighScores tempHS)
{
	dbAction = DBAction::GetHighScores;

	gv->HSVec.clear();

	wchar_t* query = const_cast<wchar_t*>(L"");

	highScores = std::move(tempHS);

	switch (highScores)
	{
	case HighScores::ThirtySeconds:
		query = const_cast<wchar_t*>(L"SELECT TOP(10) Users.Nickname, Config.PHS_30 FROM Users, Config WHERE Users.ID = Config.UserID ORDER BY Config.PHS_30 DESC");
		break;

	case HighScores::OneMinute:
		query = const_cast<wchar_t*>(L"SELECT TOP(10) Users.Nickname, Config.PHS_60 FROM Users, Config WHERE Users.ID = Config.UserID ORDER BY Config.PHS_60 DESC");
		break;

	case HighScores::TwoMinutes:
		query = const_cast<wchar_t*>(L"SELECT TOP(10) Users.Nickname, Config.PHS_120 FROM Users, Config WHERE Users.ID = Config.UserID ORDER BY Config.PHS_120 DESC");
		break;

	case HighScores::OneMistake:
		query = const_cast<wchar_t*>(L"SELECT TOP(10) Users.Nickname, Config.PHS_Mistake FROM Users, Config WHERE Users.ID = Config.UserID ORDER BY Config.PHS_Mistake DESC");
		break;
	}

	rc = SQLAllocStmt(hDbc, &hStmt);

	executeSelectQuery(gv, query);

}

unsigned int Database::getUserID(std::unique_ptr<GameVariable>& gv, std::wstring nickname)
{
	wchar_t selectQuery[] = L"SELECT ID FROM Users WHERE Nickname = ?";

	rc = SQLAllocStmt(hDbc, &hStmt);
	SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_VARCHAR, std::wcslen(nickname.c_str()), 0, const_cast<wchar_t*>(nickname.c_str()), 0, 0);

	return _wtoi(SQL_GetData(selectQuery));
}

SQLWCHAR* Database::getUserNick(std::unique_ptr<GameVariable>& gv, unsigned int userID)
{
	wchar_t selectQuery[] = L"SELECT Nickname FROM Users WHERE ID = ?";

	rc = SQLAllocStmt(hDbc, &hStmt);
	SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, std::to_string(userID).length(), 0, &userID, 0, 0);

	return SQL_GetData(selectQuery);
}

bool Database::userRegist(std::unique_ptr<GameVariable>& gv)
{
	selectUsersTable(gv);

	for (const auto& el : gv->usersVec)
	{
		if (gv->getNickname() == el.first) { dbError = DBError::UserExists; return false; }
	}

	if (insertToUsersTable(gv))
	{
		unsigned int userID = getUserID(gv, gv->getNickname());
		return true;
	}
	else { return false; }
}

bool Database::userAuth(std::unique_ptr<GameVariable>& gv)
{
	selectUsersTable(gv);

	for (const auto& el : gv->usersVec)
	{
		if (gv->getNickname() == el.first && gv->getPassword() == el.second) { return true; }
	}
	return false;
}