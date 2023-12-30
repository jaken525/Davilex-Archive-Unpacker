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
	void Open(int mode);
};

void Unpacker::ErasePathAndName(std::string& path)
{
	fileName = GetFileNameFile(path);
	filePath = GetFileNamePath(path);
}

void Unpacker::Open(int mode)
{
	if (mode == 1)
		archive.Read(filePath, fileName);
	else
		archive.Pack();
}