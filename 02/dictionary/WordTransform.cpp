#include <WordTransform.hpp>
#include <ranges>

void ToLower(std::string &s) {
	std::ranges::transform(s, s.begin(), ::tolower);
}