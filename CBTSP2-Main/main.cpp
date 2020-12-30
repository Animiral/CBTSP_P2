#include <iostream>

import cbtsp;

int main(int argc, const char* argv[])
{
	if (argc < 2) {
		std::cout << "No input file.\n";
		return 1;
	}

	auto problem = Problem::fromText(argv[1]);
	std::cout << "Problem parsed.\n";
}
