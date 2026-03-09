#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "NumberProcessing.hpp"

#include <limits>
#include <sstream>
#include <string>

//
// ----------- ReadNumbers tests -----------
//

TEST_CASE("ReadNumbers reads valid floating point numbers", "[ReadNumbers]")
{
	std::istringstream input("2.0 4 -1.5");
	auto result = ReadNumbers(input);

	REQUIRE(result.size() == 3);
	REQUIRE(result[0] == 2.0);
	REQUIRE(result[1] == 4.0);
	REQUIRE(result[2] == -1.5);
}

TEST_CASE("ReadNumbers handles empty input", "[ReadNumbers]")
{
	std::istringstream input("");
	auto result = ReadNumbers(input);

	REQUIRE(result.empty());
}

TEST_CASE("ReadNumbers throws invalid_argument for non-number", "[ReadNumbers]")
{
	std::istringstream input("abc 4.5 7");
	REQUIRE_THROWS_AS(ReadNumbers(input), std::invalid_argument);
}

TEST_CASE("ReadNumbers throws out_of_range for too large number", "[ReadNumbers]")
{
	std::ostringstream big;
	big << std::numeric_limits<double>::max() << "0"; // гарантированно overflow

	std::istringstream input(big.str());
	REQUIRE_THROWS_AS(ReadNumbers(input), std::out_of_range);
}

//
// ----------- ProcessNumbers tests -----------
//

TEST_CASE("ProcessNumbers multiplies by minimal element (positive case)", "[ProcessNumbers]")
{
	std::vector<double> numbers = { 10, 7, 4 };

	ProcessNumbers(numbers);

	REQUIRE(numbers == std::vector<double>{ 40, 28, 16 });
}

TEST_CASE("ProcessNumbers multiplies by minimal element (negative case)", "[ProcessNumbers]")
{
	std::vector<double> numbers = { 2.0, 4.0, -1.5 };

	ProcessNumbers(numbers);

	REQUIRE(numbers == std::vector<double>{ -3.0, -6.0, 2.25 });
}

TEST_CASE("ProcessNumbers works with all negative numbers", "[ProcessNumbers]")
{
	std::vector<double> numbers = { -1.5, -2.0, -3.0 };

	ProcessNumbers(numbers);

	REQUIRE(numbers == std::vector<double>{ 4.5, 6.0, 9.0 });
}

TEST_CASE("ProcessNumbers works with single element", "[ProcessNumbers]")
{
	std::vector<double> numbers = { 5.0 };

	ProcessNumbers(numbers);

	REQUIRE(numbers == std::vector<double>{ 25.0 });
}

TEST_CASE("ProcessNumbers handles empty vector safely", "[ProcessNumbers]")
{
	std::vector<double> numbers;

	REQUIRE_NOTHROW(ProcessNumbers(numbers));
	REQUIRE(numbers.empty());
}

//
// ----------- PrintSortedNumbers tests -----------
//

TEST_CASE("PrintSortedNumbers prints sorted values with 3 digits precision", "[PrintSortedNumbers]")
{
	std::vector<double> numbers = { 2.25, -6.0, -3.0 };

	std::ostringstream output;
	PrintSortedNumbers(output, numbers);

	REQUIRE(output.str() == "-6.000 -3.000 2.250 \n");
}

TEST_CASE("PrintSortedNumbers does not modify original vector", "[PrintSortedNumbers]")
{
	std::vector<double> numbers = { 3.0, 1.0, 2.0 };
	std::vector<double> original = numbers;

	std::ostringstream output;
	PrintSortedNumbers(output, numbers);

	REQUIRE(numbers == original);
}

//
// ----------- Integration test -----------
//

TEST_CASE("Full pipeline test", "[Integration]")
{
	std::istringstream input("2.0 4 -1.5abc");
	std::ostringstream output;

	auto numbers = ReadNumbers(input);
	ProcessNumbers(numbers);
	PrintSortedNumbers(output, numbers);

	REQUIRE(output.str() == "-6.000 -3.000 2.250 \n");
}
