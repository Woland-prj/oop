#include "Date.h"
#include <iostream>
#include <string>

void PrintDate(const std::string& name, const Date& d)
{
	std::cout << name << ": ";

	if (!d.IsValid())
	{
		std::cout << "INVALID\n";
		return;
	}

	std::cout << d << "\n";
	std::cout << "  Day:   " << d.GetDay() << "\n";
	std::cout << "  Month: " << Date::MonthToString(d.GetMonth()) << "\n";
	std::cout << "  Year:  " << d.GetYear() << "\n";
	std::cout << "  Weekday: " << Date::WeekDayToString(d.GetWeekDay()) << "\n";
}

int main()
{
	try
	{
		Date d1, d2;

		std::cout << "Enter first date (DD.MM.YYYY or INVALID): ";
		std::cin >> d1;

		std::cout << "Enter second date (DD.MM.YYYY or INVALID): ";
		std::cin >> d2;

		std::cout << "\n=== INPUT ===\n";
		PrintDate("Date1", d1);
		PrintDate("Date2", d2);

		std::cout << "\n=== COMPARISON ===\n";
		std::cout << "d1 == d2: " << (d1 == d2) << "\n";
		std::cout << "d1 != d2: " << (d1 != d2) << "\n";
		std::cout << "d1 < d2:  " << (d1 < d2) << "\n";
		std::cout << "d1 > d2:  " << (d1 > d2) << "\n";

		std::cout << "\n=== DIFFERENCE ===\n";
		std::cout << "d1 - d2 = " << (d1 - d2) << " days\n";

		std::cout << "\n=== INCREMENT / DECREMENT ===\n";
		Date tmp1 = d1;
		++tmp1;
		PrintDate("++d1", tmp1);

		Date tmp2 = d1;
		--tmp2;
		PrintDate("--d1", tmp2);

		std::cout << "\n=== ADD / SUB ===\n";
		Date plus = d1 + 10;
		PrintDate("d1 + 10", plus);

		Date minus = d1 - 10;
		PrintDate("d1 - 10", minus);

		std::cout << "\n=== COMPOUND ===\n";
		Date d3 = d1;
		d3 += 5;
		PrintDate("d1 += 5", d3);

		Date d4 = d1;
		d4 -= 5;
		PrintDate("d1 -= 5", d4);
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Error: " << ex.what() << "\n";
	}

	return 0;
}