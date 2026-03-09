#include "NumberProcessing.hpp"
#include <exception>
#include <iostream>
#include <vector>

int main()
{
	std::vector<double> nums;
	try
	{
		nums = ReadNumbers(std::cin);
	}
	catch (const std::exception& ex)
	{
		std::cout << "ERROR" << std::endl;
		return 0;
	}
	ProcessNumbers(nums);
	PrintSortedNumbers(std::cout, nums);
}
