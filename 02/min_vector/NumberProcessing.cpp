#include <NumberProcessing.hpp>
#include <algorithm>
#include <charconv>
#include <cstdlib>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <vector>

constexpr size_t k_precision = 3;

// Поправить 2.0 4 -1.5abc
// - 6.000 - 3.000 2.250
std::vector<double> ReadNumbers(std::istream& in)
{
	std::vector<double> res{};
	for (std::string token; in >> token;)
	{
		double num{};
		auto [ptr, ec] = std::from_chars(token.data(), token.data() + token.size(), num);

		if (ec == std::errc())
			res.push_back(num);
		else if (ec == std::errc::invalid_argument)
			throw std::invalid_argument(std::format("%s is not a floating point number", token));
		else if (ec == std::errc::result_out_of_range)
			throw std::out_of_range(std::format("%s is bigger than double", token));
	}
	return res;
};

void ProcessNumbers(std::vector<double>& numbers) noexcept
{
	if (numbers.empty())
		return;
	double min = *std::min_element(numbers.begin(), numbers.end());
	for (double& num : numbers)
		num *= min;
}

void PrintSortedNumbers(std::ostream& out, std::vector<double> numbers) noexcept
{
	if (numbers.empty())
		return;
	std::sort(numbers.begin(), numbers.end());
	out << std::setprecision(k_precision) << std::fixed;
	for (const double& num : numbers)
		out << num << ' ';
	out << std::endl;
}
