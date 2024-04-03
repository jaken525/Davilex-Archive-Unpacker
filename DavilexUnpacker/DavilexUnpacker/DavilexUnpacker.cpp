#include <iostream>

#include "unpacker.h"

int main(int argc, char* argv[])
{
	std::cout << "Choose mode:\n 1 - Unpacker\n 2 - Packer\n-> ";

	int mode = 0;
	std::cin >> mode;

	if (mode == 1)
	{
		std::string file = "";
		if (argc < 2)
			file = OpenFileName();
		else
			file = argv[1];

		if (file != "")
		{
			Unpacker unpacker(file);
			unpacker.Open(mode);
		}
	}
	else if (mode == 2)
	{
		Unpacker unpacker;
		unpacker.Open(mode);
	}

	return 1;
}
