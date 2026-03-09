#ifndef NUMBERPROCESSING_HPP
#define NUMBERPROCESSING_HPP

#include <istream>
#include <vector>

std::vector<double> ReadNumbers(std::istream& in);
void ProcessNumbers(std::vector<double>& numbers) noexcept;
void PrintSortedNumbers(std::ostream& out, std::vector<double> numbers) noexcept;

#endif // NUMBERPROCESSING_HPP
