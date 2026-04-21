#include "MyString.h"

#include <iostream>

int main()
{
	const char* testString = "Hello World!";
	MyString myTestString(testString);
	std::cout << myTestString << std::endl;
}