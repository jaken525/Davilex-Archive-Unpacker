#include "files.h"

void files::IMG::Read(std::string& filePath, std::string& fileName)
{
	indexFile.Read(filePath, fileName);

	char* buffer = NULL;
	size_t fsize = 0;
	size_t pos = 0;
	if (OpenFile(buffer, fsize, filePath + fileName + ".img"))
	{
		for (int i = 0; i < indexFile.countFiles; i++)
		{
			pos += 4;

			if (i < indexFile.countFiles)
				files[i] = ReadString(buffer, pos, indexFile.fileOffsets[i + 1] - indexFile.fileOffsets[i] - 4);
			else
				files[i] = ReadString(buffer, pos, fsize - indexFile.fileOffsets[i] - 4);
		}
	}
	if (buffer != NULL)
	{
		delete[] buffer;
		buffer = NULL;
	}
}

int files::IMG::GetCountOfFiles()
{
	return indexFile.countFiles;
}

void files::IND::Read(std::string& filePath, std::string& fileName)
{
	char* buffer = NULL;
	size_t fsize = 0;
	size_t pos = 0;

	if (OpenFile(buffer, fsize, filePath + fileName + ".ind"))
	{
		countFiles = ReadShort(buffer, pos);

		fileNames = new std::string[countFiles];
		fileOffsets = new uint32_t[countFiles];

		for (int i = 0; i < countFiles; i++)
		{
			fileNames[i] = ReadString(buffer, pos, 20);
			fileOffsets[i] = ReadLong(buffer, pos);
		}
	}
	if (buffer != NULL)
	{
		delete[] buffer;
		buffer = NULL;
	}
}