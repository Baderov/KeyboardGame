#ifndef FILE_H
#define FILE_H

#include "GameVariable.h"
#include "GameWindow.h"
#include "TGUI.h"

enum class FileAction
{
	WriteToFile, ReadFromFile, ClearFile
}; inline FileAction fileAction;

class CustomFile
{
private:
	std::wfstream file;
	std::wstring line;
public:
	CustomFile(std::wstring path, FileAction fileAction);
	~CustomFile();

	void read(std::vector<std::wstring>& vec);
	void write(std::wstring str);

};

void clearFile(std::wstring filePath);

void shuffleDictionaryVec(std::unique_ptr<GameVariable>& gv);

void fillDictionaryVec(std::unique_ptr<GameVariable>& gv);

#endif