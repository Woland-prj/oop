#include "MyArray.h"
#include <iostream>

int main()
{
	MyArray<std::string> strings;

	strings.PushBack("Hello");
	strings.PushBack("World");

	std::cout << "Strings:\n";
	for (const auto& s : strings)
	{
		std::cout << s << "\n";
	}

	MyArray<double> numbers;

	numbers.PushBack(1.1);
	numbers.PushBack(2.2);
	numbers.PushBack(3.3);

	std::cout << "\nNumbers:\n";
	for (auto it = numbers.rbegin(); it != numbers.rend(); ++it)
	{
		std::cout << *it << "\n";
	}

	MyArray<int> ints;
	ints.PushBack(10);
	ints.PushBack(20);

	MyArray<double> converted;
	converted = ints;

	std::cout << "\nConverted:\n";
	for (const auto& v : converted)
	{
		std::cout << v << "\n";
	}
}
