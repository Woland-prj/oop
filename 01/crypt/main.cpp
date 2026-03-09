#include "CryptApp.hpp"
#include <exception>
#include <iostream>

int main(int argc, char* argv[])
{
	try
	{
		RunCryptApp(argc, argv);
	}
	catch (const std::exception& ex)
	{
		std::cout << "ERROR:\n\t" << ex.what() << "\nSee help:\n\tcrypt --help" << std::endl;
		return 1;
	}

	return 0;
}
