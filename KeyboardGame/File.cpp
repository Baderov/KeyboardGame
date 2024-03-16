#include "pch.h"
#include "File.h"

CustomFile::CustomFile(std::wstring path, FileAction _fileAction)
{
	fileAction = std::move(_fileAction);

	line = L"";
	switch (fileAction)
	{
	case FileAction::WriteToFile:
		file.open(path, std::ios::out | std::ios::app);
		break;
	case FileAction::ReadFromFile:
		file.open(path, std::ios::in);
		break;
	case FileAction::ClearFile:
		file.open(path, std::ios::out | std::ios::trunc);
		break;
	}

	if (!file.is_open()) { DEBUG_MSG(L"Failed to open file - " << path); }
}

CustomFile::~CustomFile() { file.close(); }

void CustomFile::write(std::wstring str) { file << str; }

void CustomFile::read(std::vector<std::wstring>& vec)
{
	while (std::getline(file, line))
	{
		vec.emplace_back(std::move(line));
	}
}

void clearFile(std::wstring filePath)
{
	CustomFile file(filePath, FileAction::ClearFile);
}

void shuffleDictionaryVec(std::unique_ptr<GameVariable>& gv)
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(gv->dictionaryVector.begin(), gv->dictionaryVector.end(), g);
}

void fillDictionaryVec(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw)
{
	gv->dictionaryVector.clear();

	std::wstring filePath = L"Documents\\Dictionary.txt";

	CustomFile file(filePath, FileAction::ReadFromFile);
	file.read(gv->dictionaryVector);

	shuffleDictionaryVec(gv);
}