#include "Filling.hpp"
#include "Image.hpp"
#include <fstream>
#include <iostream>

constexpr std::string_view k_helpFlag = "-h";

void PrintHelp()
{
	std::cout << "Usage:\n"
				 "  fill <input file> <output file>\n"
				 "  fill\n"
				 "  fill -h\n\n"
				 "Input image max size: 100x100\n";
}

void RunFill(std::istream& in, std::ostream& out)
{
	Image img = LoadImage(in);

	Fill(img);

	SaveImage(img, out);
}

void RunFillApp(int argc, char* argv[])
{
	if (argc == 2 && std::string_view(argv[1]) == k_helpFlag)
	{
		PrintHelp();
		return;
	}

	if (argc == 1)
	{
		RunFill(std::cin, std::cout);
		return;
	}

	if (argc == 3)
	{
		std::ifstream in(argv[1]);
		if (!in)
			throw std::runtime_error("Cannot open input file");

		std::ofstream out(argv[2]);
		if (!out)
			throw std::runtime_error("Cannot open output file");

		RunFill(in, out);
		return;
	}

	throw std::invalid_argument("Invalid arguments");
}
