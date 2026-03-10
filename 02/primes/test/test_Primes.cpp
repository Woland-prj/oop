#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "Primes.hpp"

#include <algorithm>
#include <set>
#include <vector>

// Helper function: naive primality check for test validation
static bool IsPrimeNaive(int n)
{
	if (n < 2)
		return false;
	if (n == 2)
		return true;
	if (n % 2 == 0)
		return false;
	for (int i = 3; i * i <= n; i += 2)
	{
		if (n % i == 0)
			return false;
	}
	return true;
}

// create set from initializer list
static std::set<int> MakePrimeSet(std::initializer_list<int> primes)
{
	return std::set<int>(primes);
}

// Boundary values, basic cases
TEST_CASE("GeneratePrimeNumbersSet returns empty set for upperBound < 2", "[boundary]")
{
	REQUIRE(GeneratePrimeNumbersSet(-100).empty());
	REQUIRE(GeneratePrimeNumbersSet(-1).empty());
	REQUIRE(GeneratePrimeNumbersSet(0).empty());
	REQUIRE(GeneratePrimeNumbersSet(1).empty());
}

TEST_CASE("GeneratePrimeNumbersSet handles smallest primes correctly", "[basic]")
{
	REQUIRE(GeneratePrimeNumbersSet(2) == MakePrimeSet({ 2 }));
	REQUIRE(GeneratePrimeNumbersSet(3) == MakePrimeSet({ 2, 3 }));
	REQUIRE(GeneratePrimeNumbersSet(4) == MakePrimeSet({ 2, 3 }));
	REQUIRE(GeneratePrimeNumbersSet(5) == MakePrimeSet({ 2, 3, 5 }));
}

TEST_CASE("GeneratePrimeNumbersSet returns correct primes up to 10", "[basic]")
{
	auto primes = GeneratePrimeNumbersSet(10);
	REQUIRE(primes == MakePrimeSet({ 2, 3, 5, 7 }));
	REQUIRE(primes.size() == 4);
}

TEST_CASE("GeneratePrimeNumbersSet returns correct primes up to 20", "[basic]")
{
	auto primes = GeneratePrimeNumbersSet(20);
	REQUIRE(primes == MakePrimeSet({ 2, 3, 5, 7, 11, 13, 17, 19 }));
	REQUIRE(primes.size() == 8);
}

// Correctness: all returned numbers are actually prime
TEST_CASE("GeneratePrimeNumbersSet returns only prime numbers", "[correctness]")
{
	for (int bound : { 30, 50, 100 })
	{
		auto primes = GeneratePrimeNumbersSet(bound);

		for (int p : primes)
		{
			REQUIRE(IsPrimeNaive(p));
			REQUIRE(p <= bound);
		}

		for (int i = 2; i <= bound; ++i)
		{
			if (IsPrimeNaive(i))
			{
				REQUIRE(primes.contains(i));
			}
		}
	}
}

TEST_CASE("GeneratePrimeNumbersSet does not include composite numbers", "[correctness]")
{
	auto primes = GeneratePrimeNumbersSet(100);

	for (int composite : { 4, 6, 8, 9, 10, 12, 15, 20, 25, 50, 100 })
	{
		REQUIRE_FALSE(primes.contains(composite));
	}
}

// Prime counting function validation (known pi(x) values)
TEST_CASE("GeneratePrimeNumbersSet returns correct count for known bounds", "[pi-function]")
{
	SECTION("Small bounds")
	{
		REQUIRE(GeneratePrimeNumbersSet(10).size() == 4);
		REQUIRE(GeneratePrimeNumbersSet(25).size() == 9);
		REQUIRE(GeneratePrimeNumbersSet(50).size() == 15);
	}

	SECTION("Medium bounds")
	{
		REQUIRE(GeneratePrimeNumbersSet(100).size() == 25);
		REQUIRE(GeneratePrimeNumbersSet(500).size() == 95);
		REQUIRE(GeneratePrimeNumbersSet(1000).size() == 168);
	}

	SECTION("Larger bounds")
	{
		REQUIRE(GeneratePrimeNumbersSet(10000).size() == 1229);
		REQUIRE(GeneratePrimeNumbersSet(100000).size() == 9592);
	}
}

// Container properties verification
TEST_CASE("GeneratePrimeNumbersSet returns elements in ascending order", "[container]")
{
	auto primes = GeneratePrimeNumbersSet(50);

	int prev = -1;
	for (int p : primes)
	{
		REQUIRE(p > prev);
		prev = p;
	}
}

TEST_CASE("GeneratePrimeNumbersSet returns unique elements", "[container]")
{
	auto primes = GeneratePrimeNumbersSet(100);

	std::vector<int> vec(primes.begin(), primes.end());
	std::sort(vec.begin(), vec.end());
	vec.erase(std::unique(vec.begin(), vec.end()), vec.end());

	REQUIRE(static_cast<int>(vec.size()) == static_cast<int>(primes.size()));
}

// Maximum bound acceptance test (100 million)
TEST_CASE("GeneratePrimeNumbersSet handles maximum bound 100000000", "[performance][acceptance]")
{
	auto primes = GeneratePrimeNumbersSet(100000000);

	REQUIRE(primes.size() == 5761455);

	REQUIRE(*primes.begin() == 2);
	REQUIRE(*primes.rbegin() == 99999989);
}

// Additional correctness checks
TEST_CASE("GeneratePrimeNumbersSet correctly excludes even numbers greater than 2", "[correctness]")
{
	auto primes = GeneratePrimeNumbersSet(100);

	for (int even = 4; even <= 100; even += 2)
	{
		REQUIRE_FALSE(primes.contains(even));
	}
	REQUIRE(primes.contains(2));
}

TEST_CASE("GeneratePrimeNumbersSet correctly handles squares of primes", "[correctness]")
{
	auto primes = GeneratePrimeNumbersSet(200);

	for (int p : { 2, 3, 5, 7, 11, 13 })
	{
		int square = p * p;
		if (square <= 200)
		{
			REQUIRE_FALSE(primes.contains(square));
		}
	}
}

TEST_CASE("GeneratePrimeNumbersSet includes twin primes correctly", "[correctness]")
{
	auto primes = GeneratePrimeNumbersSet(100);

	std::vector<std::pair<int, int>> twins = {
		{ 3, 5 }, { 5, 7 }, { 11, 13 }, { 17, 19 },
		{ 29, 31 }, { 41, 43 }, { 59, 61 }, { 71, 73 }
	};

	for (const auto& [p1, p2] : twins)
	{
		REQUIRE(primes.contains(p1));
		REQUIRE(primes.contains(p2));
	}
}
