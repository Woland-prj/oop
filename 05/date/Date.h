#ifndef DATE_H
#define DATE_H

#include <iosfwd>
#include <string_view>

enum class Month
{
	JANUARY = 1,
	FEBRUARY,
	MARCH,
	APRIL,
	MAY,
	JUNE,
	JULY,
	AUGUST,
	SEPTEMBER,
	OCTOBER,
	NOVEMBER,
	DECEMBER
};

enum class WeekDay
{
	SUNDAY = 0,
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY
};

class Date
{
public:
	Date(unsigned day, Month month, unsigned year);
	explicit Date(unsigned timestamp);
	Date();

	unsigned GetDay() const;
	Month GetMonth() const;
	unsigned GetYear() const;
	WeekDay GetWeekDay() const;
	bool IsValid() const;

	Date& operator++();
	Date operator++(int);
	Date& operator--();
	Date operator--(int);

	Date operator+(int days) const;
	Date operator-(int days) const;
	long long operator-(const Date& other) const;
	Date& operator+=(int days);
	Date& operator-=(int days);

	bool operator==(const Date& other) const;
	bool operator!=(const Date& other) const;
	bool operator<(const Date& other) const;
	bool operator>(const Date& other) const;
	bool operator<=(const Date& other) const;
	bool operator>=(const Date& other) const;

	friend std::ostream& operator<<(std::ostream& os, const Date& date);
	friend std::istream& operator>>(std::istream& is, Date& date);

	friend Date operator+(int days, const Date& date);

	static std::string_view MonthToString(Month month);
	static std::string_view WeekDayToString(WeekDay weekDay);

private:
	unsigned m_daysSinceEpoch;
	bool m_valid;
};

Date operator+(int days, const Date& date);

#endif // DATE_H