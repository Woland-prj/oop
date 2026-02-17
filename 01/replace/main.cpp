#include "runner/ReplaceApp.h"
#include <exception>
#include <iostream>

// Переименовать папки
int main(int argc, char* argv[])
{
	try
	{
		ReplaceApp::Run(argc, argv);
		return 0;
	}
	catch (const std::exception& ex)
	{
		std::cout << "ERROR" << std::endl;
		return (argc > 1) ? 1 : 0;
	}
}
