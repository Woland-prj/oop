#include "ExpressionParser.hpp"

#include <iostream>
#include <sstream>
#include <string>

int main()
{
	std::string line;
	while (std::getline(std::cin, line))
	{
		if (line.empty())
			continue;
		try
		{
			std::istringstream iss(line);
			int64_t result = EvalPrefixExpression(iss);
			std::cout << result << '\n';
		}
		catch (const std::exception&)
		{
			std::cout << "ERROR\n";
		}
	}
	return 0;
}
