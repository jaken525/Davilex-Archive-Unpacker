#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <windows.h>
#include <shlwapi.h>
#include <filesystem>

std::string OpenFileName(HWND owner = NULL, uint32_t flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY)
{
	std::string filename(MAX_PATH, '\0');
	OPENFILENAME ofn = { };

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = "Davilex Archive File (*.ind)\0*.ind\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = &filename[0];
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a File";
	ofn.Flags = flags;

	if (!GetOpenFileName(&ofn))
		return "";

	return filename;
}

std::string GetFileNamePath(const std::string str)
{
	size_t found;
	std::string strt;
	found = str.find_last_of("/\\");

	if (found != std::string::npos)
	{
		strt = str.substr(0, found);

		return (strt + "\\");
	}
	else 
		return "";
}

std::string GetFileNameFile(const std::string str)
{
	size_t found;
	std::string strt;
	found = str.find_last_of("/\\");

	if (found < str.size())
	{
		strt = str.substr(found + 1, -1);
		found = strt.find(".");

		if (found < strt.size())
			strt = strt.substr(0, found);
	}
	else 
		strt = str;

	size_t lastdot = strt.find_last_of(".");
	if (lastdot == std::string::npos) 
		return strt;

	return strt.substr(0, lastdot);
}

std::string ReadStringWithout(char*& f, size_t& pos, int size)
{
	std::string result = "";
	bool isWriting = true;

	for (int i = 0; i < size; i++)
	{
		if (f[pos] != 0 && isWriting)
			result += f[pos];
		else
			isWriting = false;
		pos += 1;
	}

	return result;
}

std::string ReadString(char*& f, size_t& pos, int size)
{
	std::string result = "";

	for (int i = 0; i < size; i++)
	{
		result += f[pos];
		pos += 1;
	}

	return result;
}

unsigned short ReadShort(char*& f, size_t& pos)
{
	unsigned short result = {
		(unsigned int)((uint8_t)f[pos] * 0x00000001) + \
		(unsigned int)((uint8_t)f[pos + 1] * 0x00000100)
	};

	pos += 2;
	return result;
}

unsigned long ReadLong(char*& f, size_t& pos)
{
	unsigned long result = {
		(unsigned int)((uint8_t)f[pos] * 0x00000001) + \
		(unsigned int)((uint8_t)f[pos + 1] * 0x00000100) + \
		(unsigned int)((uint8_t)f[pos + 2] * 0x00010000) + \
		(unsigned int)((uint8_t)f[pos + 3] * 0x01000000)
	};

	pos += 4;
	return result;
}

uint8_t ReadSingle(char*& f, size_t& pos)
{
	uint8_t result = (uint8_t)f[pos];
	pos += 1;

	return result;
}

float ReadFloat(char*& f, size_t& pos)
{
	float result = 0.0;
	unsigned long b = ReadLong(f, pos);
	memcpy(&result, &b, 4);

	return result;
}

bool OpenFile(char*& f, size_t& fsize, std::string filename)
{
	HANDLE hFile = CreateFile(
		filename.c_str(),		// file to open
		GENERIC_READ,			// open for reading
		FILE_SHARE_READ,		// share for reading
		NULL,					// default security
		OPEN_EXISTING,			// existing file only
		FILE_ATTRIBUTE_NORMAL,	// normal file
		NULL					// no attr. template
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "Failed to Open File\n";
		return false;
	}

	fsize = GetFileSize(hFile, NULL);
	if (fsize == 0)
	{
		std::cout << "Failed to read file. File is Empty?\n";
		return false;
	}

	f = new char[fsize];
	unsigned long dwBytesRead = 0;
	if (ReadFile(hFile, f, fsize, &dwBytesRead, NULL) == FALSE || dwBytesRead != fsize)
	{
		std::cout << "Failed to copy file into memory\n";
		
		fsize = 0;
		delete[] f;
		f = NULL;

		CloseHandle(hFile);
		return false;
	}
	CloseHandle(hFile);
}