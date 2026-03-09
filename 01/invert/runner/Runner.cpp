#include "Runner.h"
#include "matrix/Matrix.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

void RunHelp()
{
	std::cout << "invert - 3x3 matrix inversion utility\n"
				 "\n"
				 "Usage:\n"
				 "  invert             # Read matrix from stdin\n"
				 "  invert <file>      # Read matrix from file\n"
				 "  invert -h          # Show this help\n"
				 "\n"
				 "Input format:\n"
				 "  3 rows, 3 values per row\n"
				 "  Values separated by TAB\n"
				 "\n"
				 "Output:\n"
				 "  Inverted matrix with precision 3 decimal places\n";
}

void DoInvert(std::istream& input)
{
	Matrix<3> matrix(input);

	Matrix<3> inverted = matrix.Invert();
	std::cout << inverted;
}

void RunCli()
{
	DoInvert(std::cin);
}

void RunFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
		throw std::runtime_error("Invalid matrix format");

	DoInvert(file);
}

void Run(int argc, char* argv[])
{
	if (argc == 1)
	{
		RunCli();
		return;
	}

	if (argc == 2)
	{
		std::string arg = argv[1];

		if (arg == "-h")
		{
			RunHelp();
			return;
		}

		RunFile(arg); // Переименовать
		return;
	}

	throw std::invalid_argument("Invalid matrix format");
}
