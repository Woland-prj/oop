#include "DictionaryApp.hpp"

#include <exception>
#include <iostream>

int main(int argc, char* argv[])
{
	try
	{
		RunDictionaryApp(argc, argv);
	} catch (const std::exception& ex)
	{
		std::cerr << "Error: " << ex.what();
		return 1;
	}
	return 0;
}