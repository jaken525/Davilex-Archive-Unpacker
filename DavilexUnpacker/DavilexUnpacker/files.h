#pragma once
#include "BinaryFileWork.h"

#include <zlib.h>

namespace files
{
	const std::string archiveError = "Maybe this is not a Davilex Archive. Davilex archives must have 2 files: img and ind.";

	struct IND
	{
		int countFiles = NULL;

		std::string* fileNames = NULL;
		uint32_t* fileOffsets = NULL;

		IND()
		{
			fileNames = NULL;
			fileOffsets = NULL;
		}

		~IND()
		{
			if (fileNames != NULL) fileNames = NULL;
			if (fileOffsets != NULL) fileOffsets = NULL;
		}

		bool Read(std::string& filePath, std::string& fileName);
	};

	struct	IMG
	{
		bool isCompressed = false;

		uLong* fileSize = NULL;

		IND indexFile;

		IMG()
		{
			fileSize = NULL;
		}

		~IMG()
		{
			if (fileSize != NULL) fileSize = NULL;
		}

		void Read(std::string& filePath, std::string& fileName);
		void AnalysisArchiveType(char* buffer);
		int GetCountOfFiles();
	};
}

void files::IMG::Read(std::string& filePath, std::string& fileName)
{
	if (std::filesystem::exists(filePath + fileName + ".img"))
	{
		if (indexFile.Read(filePath, fileName))
		{
			char* buffer = NULL;
			size_t fsize = 0;
			size_t pos = 0;
			if (OpenFile(buffer, fsize, filePath + fileName + ".img"))
			{
				std::cout << "Analysis of the archive type..." << std::endl;
				AnalysisArchiveType(buffer);

				std::filesystem::create_directory(filePath + fileName);
				if (isCompressed)
				{
					std::cout << "[Compressed file reading mode]" << std::endl;

					fileSize = new uLong[indexFile.countFiles];

					for (int i = 0; i < indexFile.countFiles; i++)
					{
						fileSize[i] = ReadLong(buffer, pos);
						std::cout << "Writting " << indexFile.fileNames[i] << std::endl;

						std::string currentZippedFile = "";
						int currentFileSize = 0;

						if (i + 1 < indexFile.countFiles)
							currentFileSize = indexFile.fileOffsets[i + 1] - indexFile.fileOffsets[i] - 4;
						else
							currentFileSize = fsize - indexFile.fileOffsets[i] - 4;

						currentZippedFile = ReadString(buffer, pos, currentFileSize);

						char* unzippedFile = new char[fileSize[i]];
						uncompress((Bytef*)unzippedFile, &fileSize[i], (Bytef*)currentZippedFile.c_str(), currentFileSize);

						std::ofstream newFile(filePath + fileName + "\\" + indexFile.fileNames[i], std::ios::binary);
						newFile.write(unzippedFile, fileSize[i]);
						newFile.close();

						delete[] unzippedFile;
					}
				}
				else
				{
					std::cout << "[Normal file reading mode]" << std::endl;

					for (int i = 0; i < indexFile.countFiles; i++)
					{
						std::cout << "Writting " << indexFile.fileNames[i] << std::endl;

						std::string currentFile = "";
						int currentFileSize = 0;

						if (i + 1 < indexFile.countFiles)
							currentFileSize = indexFile.fileOffsets[i + 1] - indexFile.fileOffsets[i];
						else
							currentFileSize = fsize - indexFile.fileOffsets[i];

						currentFile = ReadString(buffer, pos, currentFileSize);

						std::ofstream newFile(filePath + fileName + "\\" + indexFile.fileNames[i], std::ios::binary);
						newFile << currentFile;
						newFile.close();
					}
				}
			}
			if (buffer != NULL)
			{
				delete[] buffer;
				buffer = NULL;
			}
		}
		else
			std::cout << archiveError << std::endl;
	}
	else
		std::cout << archiveError << std::endl;
}

void files::IMG::AnalysisArchiveType(char* buffer)
{
	size_t pos = 4;

	int check = ReadShort(buffer, pos);
	if (check == 55928)
		isCompressed = true;
}

int files::IMG::GetCountOfFiles()
{
	return indexFile.countFiles;
}

bool files::IND::Read(std::string& filePath, std::string& fileName)
{
	bool success = false;

	char* buffer = NULL;
	size_t fsize = 0;
	size_t pos = 0;

	if (OpenFile(buffer, fsize, filePath + fileName + ".ind"))
	{
		countFiles = ReadShort(buffer, pos);

		if (ReadLong(buffer, pos) != 2)
		{
			pos -= 4;

			fileNames = new std::string[countFiles];
			fileOffsets = new uint32_t[countFiles];

			for (int i = 0; i < countFiles; i++)
			{
				fileNames[i] = ReadString(buffer, pos, 20);
				fileOffsets[i] = ReadLong(buffer, pos);
			}

			success = true;
		}
	}
	if (buffer != NULL)
	{
		delete[] buffer;
		buffer = NULL;
	}

	return success;
}