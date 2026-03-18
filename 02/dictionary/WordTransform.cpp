#include <WordTransform.hpp>

// Обрабатывать русские символы отднльно
void ToLower(std::string& s)
{
	std::ranges::transform(s, s.begin(), ::tolower);
}
