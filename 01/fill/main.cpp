#include "FillApp.hpp"
#include <exception>
#include <iostream>

int main(int argc, char* argv[])
{
	try
	{
		RunFillApp(argc, argv);
	}
	catch (const std::exception& ex)
	{
		std::cout << "ERROR\n";

		if (argc == 1)
			return 0;

		return 1;
	}
}
