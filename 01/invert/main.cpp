#include "runner/Runner.h"
#include <exception>
#include <iostream>

// тест на 4x4
int main(int argc, char* argv[])
{
	try
	{
		Run(argc, argv); // Сменить назваеме
		return 0;
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what();
		return 1;
	}
}
