#include "RegexpApp.hpp"
#include <exception>
#include <iostream>

int main()
{
	try
	{
		RunRegexpApp(std::cin, std::cout);
	}
	catch (const std::exception& ex)
	{
		std::cout << "ERROR" << std::endl;
	}
	return 0;
}
