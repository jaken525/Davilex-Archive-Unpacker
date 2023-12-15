#pragma once
#include "files.h"

class Unpacker
{
private:
	files::IMG archive;

	std::string filePath;
	std::string fileName;
	std::string currentPath;

public:
	Unpacker()
	{
		filePath = "";
		fileName = "";
	}

	Unpacker(std::string& fullPath)
	{
		currentPath = std::filesystem::current_path().string();
		ErasePathAndName(fullPath);
	}

	~Unpacker() { }

	void ErasePathAndName(std::string& path);
	void Open();
};

void Unpacker::ErasePathAndName(std::string& path)
{
	fileName = GetFileNameFile(path);
	filePath = GetFileNamePath(path);
}

void Unpacker::Open()
{
	archive.Read(filePath, fileName);

}