#include "FindMax.hpp"
#include <iostream>
#include <string>

struct Athlete
{
	std::string fio;
	double height;
	double weight;
};

std::ostream& operator<<(std::ostream& os, const Athlete& a)
{
	os << "ФИО: " << a.fio
	   << " | Рост: " << a.height << "м"
	   << " | Вес: " << a.weight << "кг";
	return os;
}

int main()
{
	std::vector<Athlete> team = {
		{ "Иванов И.И.", 1.75, 70.0 },
		{ "Петров П.П.", 1.95, 85.0 },
		{ "Сидоров С.С.", 1.82, 78.0 },
		{ "Кузнецов К.К.", 1.90, 92.0 }
	};

	Athlete tallest;
	if (FindMax(team, tallest, [](const Athlete& a, const Athlete& b) {
			return a.height < b.height;
		}))
	{
		std::cout << "Спортсмен с максимальным ростом:\n"
				  << tallest << "\n\n";
	}

	Athlete heaviest;
	if (FindMax(team, heaviest, [](const Athlete& a, const Athlete& b) {
			return a.weight < b.weight;
		}))
	{
		std::cout << "Спортсмен с максимальным весом:\n"
				  << heaviest << "\n";
	}

	return 0;
}
