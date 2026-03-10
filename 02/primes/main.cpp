#include "Primes.hpp"
#include <charconv>
#include <exception>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>

constexpr int k_maxUpperBound = 100000000;

int ParseUpperBound(const std::string& argStr)
{
	if (argStr.empty())
		throw std::invalid_argument("Argument is empty");
	int num{};
	auto [ptr, ec] = std::from_chars(argStr.data(), argStr.data() + argStr.size(), num);
	if (ec == std::errc::invalid_argument || ptr != argStr.data() + argStr.size())
		throw std::invalid_argument("Argument is not number");
	else if (ec == std::errc::result_out_of_range || num < 0 || num > k_maxUpperBound)
		throw std::out_of_range("Upper bound must be in range 0..100000000");

	return num;
}

void PrintPrimes(const std::set<int>& primes)
{
	for (int p : primes)
		std::cout << p << '\n';
}

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
			throw std::invalid_argument("Invalid arguments");

		int upperBound = ParseUpperBound(argv[1]);

		std::set<int> primes = GeneratePrimeNumbersSet(upperBound);

		std::cout << "Count: " << primes.size() << '\n';
		PrintPrimes(primes);
	}
	catch (const std::exception& ex)
	{
		std::cout << "ERROR\n";
		return 1;
	}

	return 0;
}
