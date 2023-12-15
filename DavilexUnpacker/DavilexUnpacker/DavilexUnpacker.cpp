#include <iostream>

#include "unpacker.h"

int main(int argc, char* argv[])
{
	std::string file = "";
	if (argc < 2)
		file = OpenFileName();
	else
		file = argv[1];

	if (file != "")
	{
		Unpacker unpacker(file);
		unpacker.Open();
	}

	return 0;
}
