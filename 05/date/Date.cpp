#include "Date.h"
#include <array>
#include <chrono>
#include <format>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string_view>

constexpr unsigned FIRST_YEAR = 1970;
constexpr unsigned FINAL_YEAR = 9999;
constexpr unsigned MONTH_COUNT = 12;
constexpr unsigned WEEKDAY_COUNT = 7;

// https://howardhinnant.github.io/date_algorithms.html
constexpr unsigned HINNANT_EPOCH_SHIFT = 719468; // дни от 0000-03-01 до 1970-01-01
constexpr unsigned GREGORIAN_ERA_DAYS = 146097; // дней в 400-летнем цикле
constexpr unsigned DAYS_PER_4_YEARS = 1460; // 4×365 + 1 високосный
constexpr unsigned DAYS_PER_100_YEARS = 36524; // 100×365 + 24 − 1 столетие
constexpr unsigned MONTH_FORMULA_NUM = 153; // коэффициент в формуле месяца
constexpr unsigned MONTH_FORMULA_DEN = 5; // делитель в формуле месяца

// 01.01.1970 - четверг
constexpr unsigned EPOCH_WEEKDAY = static_cast<unsigned>(WeekDay::THURSDAY);

constexpr std::array<unsigned, MONTH_COUNT> DAYS_IN_MONTH = {
	31, 28, 31, 30, 31, 30,
	31, 31, 30, 31, 30, 31
};

constexpr std::array<std::string_view, MONTH_COUNT> MONTH_NAMES = {
	"January", "February", "March", "April", "May", "June",
	"July", "August", "September", "October", "November", "December"
};

constexpr std::array<std::string_view, WEEKDAY_COUNT> WEEKDAY_NAMES = {
	 "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

constexpr std::array<unsigned, MONTH_COUNT> DAYS_BEFORE_MONTH = {
	0, 31, 59, 90, 120, 151,
	181, 212, 243, 273, 304, 334
};

constexpr bool IsValidYear(unsigned year)
{
	return year >= FIRST_YEAR && year <= FINAL_YEAR;
}

constexpr bool IsValidMonth(Month month)
{
	return month >= Month::JANUARY && month <= Month::DECEMBER;
}

constexpr bool IsValidWeekDay(WeekDay weekDay)
{
	return weekDay >= WeekDay::SUNDAY && weekDay <= WeekDay::SATURDAY;
}

constexpr bool IsLeap(unsigned year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

constexpr unsigned DaysInMonth(Month month, unsigned year)
{
	const unsigned idx = static_cast<unsigned>(month) - 1;
	if (month == Month::FEBRUARY && IsLeap(year))
		return 29;
	return DAYS_IN_MONTH[idx];
}

constexpr unsigned LeapYearsBefore(unsigned year)
{
	return (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400;
}

constexpr unsigned DaysToYearStart(unsigned year)
{
	const unsigned years = year - FIRST_YEAR;
	const unsigned leaps = LeapYearsBefore(year) - LeapYearsBefore(FIRST_YEAR);
	return years * 365 + leaps;
}

constexpr unsigned MaxDays()
{
	return DaysToYearStart(FINAL_YEAR + 1) - 1;
}

std::string_view Date::MonthToString(Month month)
{
	if (!IsValidMonth(month))
		throw std::invalid_argument("Invalid month");
	return MONTH_NAMES[static_cast<unsigned>(month) - 1];
}

std::string_view Date::WeekDayToString(WeekDay weekDay)
{
	if (!IsValidWeekDay(weekDay))
		throw std::invalid_argument("Invalid week day");
	return WEEKDAY_NAMES[static_cast<unsigned>(weekDay)];
}

constexpr unsigned ToTimestamp(unsigned day, Month month, unsigned year)
{
	if (!IsValidYear(year))
		throw std::out_of_range(std::format("Year {} not in [{}..{}]", year, FIRST_YEAR, FINAL_YEAR));

	if (!IsValidMonth(month))
		throw std::invalid_argument(std::format("Invalid month {}", static_cast<unsigned>(month)));

	const unsigned maxDay = DaysInMonth(month, year);
	if (day < 1 || day > maxDay)
		throw std::invalid_argument(std::format("Invalid day {} for {} {}", day, Date::MonthToString(month), year));

	int y = static_cast<int>(year);
	const auto m = static_cast<unsigned>(month);
	const unsigned d = day;

	// Сдвиг: январь/февраль считаем концом предыдущего года
	y -= (m <= 2);

	// Эра (400 лет)
	const int era = (y >= 0 ? y : y - 399) / 400;
	const auto yoe = static_cast<unsigned>(y - era * 400); // [0..399]

	// День года (начало с марта)
	const unsigned mp = m + (m > 2 ? -3 : 9);
	const unsigned doy = (153 * mp + 2) / 5 + d - 1; // [0..365]

	// День эпохи
	const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy; // [0..146096]

	// Финальный timestamp (эпоха 1970)
	return era * GREGORIAN_ERA_DAYS + doe - HINNANT_EPOCH_SHIFT;
}

// Алгоритм: Howard Hinnant civil_from_days — O(1), без циклов
static std::tuple<unsigned, Month, unsigned> FromTimestamp(unsigned days)
{
	// Сдвиг эпохи: 1970-01-01 → 0000-03-01 (внутренняя точка отсчёта)
	unsigned z = days + HINNANT_EPOCH_SHIFT;

	// Эра: 400-летние григорианские циклы
	const unsigned era = z / GREGORIAN_ERA_DAYS;
	const unsigned doe = z - era * GREGORIAN_ERA_DAYS; // день внутри эры [0..146096]

	// Год внутри эры [0..399] — инверсия формулы расчёта doe
	const unsigned yoe = (doe - doe / DAYS_PER_4_YEARS + doe / DAYS_PER_100_YEARS - doe / GREGORIAN_ERA_DAYS) / 365;
	const unsigned y = yoe + era * 400; // гражданский год (пока с мартом первым)

	// День года [0..365], где 0 = 1 марта
	const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);

	// Месяц во внутренней нумерации: [0..11] → [март..февраль]
	const unsigned mp = (MONTH_FORMULA_DEN * doy + 2) / MONTH_FORMULA_NUM;

	// День месяца [1..31]
	const unsigned d = doy - (MONTH_FORMULA_NUM * mp + 2) / MONTH_FORMULA_DEN + 1;

	// Конверсия месяца: [0..11]/[мар..фев] → [1..12]/[янв..дек]
	const unsigned m = (mp < 10) ? mp + 3 : mp - 9;

	// Коррекция года: январь и февраль принадлежат предыдущему «внутреннему» году
	const unsigned year = y + (m <= 2 ? 1 : 0);

	return { d, static_cast<Month>(m), year };
}

Date::Date(unsigned day, Month month, unsigned year)
	: m_daysSinceEpoch(ToTimestamp(day, month, year))
	, m_valid(true)
{
	if (m_daysSinceEpoch > MaxDays())
		throw std::out_of_range("Date beyond 31.12.9999");
}

Date::Date(unsigned timestamp)
	: m_daysSinceEpoch(timestamp)
	, m_valid(timestamp <= MaxDays())
{
	if (!m_valid)
		throw std::out_of_range("Timestamp out of valid range");
}

Date::Date()
	: m_daysSinceEpoch(0)
	, m_valid(true)
{
}

unsigned Date::GetDay() const
{
	if (!m_valid)
		return 0;
	auto [day, month, year] = FromTimestamp(m_daysSinceEpoch);
	return day;
}

Month Date::GetMonth() const
{
	if (!m_valid)
		return Month::JANUARY;
	auto [day, month, year] = FromTimestamp(m_daysSinceEpoch);
	return month;
}

unsigned Date::GetYear() const
{
	if (!m_valid)
		return FIRST_YEAR;
	auto [day, month, year] = FromTimestamp(m_daysSinceEpoch);
	return year;
}

WeekDay Date::GetWeekDay() const
{
	if (!m_valid)
		return WeekDay::SUNDAY;
	return static_cast<WeekDay>((EPOCH_WEEKDAY + m_daysSinceEpoch) % 7);
}

bool Date::IsValid() const
{
	return m_valid;
}

Date& Date::operator++()
{
	if (!m_valid)
		return *this;
	if (m_daysSinceEpoch >= MaxDays())
	{
		m_valid = false;
		return *this;
	}
	++m_daysSinceEpoch;
	return *this;
}

Date Date::operator++(int)
{
	const Date tmp = *this;
	++(*this);
	return tmp;
}

Date& Date::operator--()
{
	if (!m_valid)
		return *this;
	if (m_daysSinceEpoch == 0)
	{
		m_valid = false;
		return *this;
	}
	--m_daysSinceEpoch;
	return *this;
}

Date Date::operator--(int)
{
	const Date tmp = *this;
	--(*this);
	return tmp;
}

Date Date::operator+(int days) const
{
	if (!m_valid)
		return *this;

	const long long newDays = static_cast<long long>(m_daysSinceEpoch) + days;

	if (newDays < 0 || newDays > static_cast<long long>(MaxDays()))
		return {};
	return Date(static_cast<unsigned>(newDays));
}

Date operator+(int days, const Date& date)
{
	return date + days;
}

Date Date::operator-(int days) const
{
	return *this + (-days);
}

long long Date::operator-(const Date& other) const
{
	if (!m_valid || !other.m_valid)
		return 0;
	return static_cast<long long>(m_daysSinceEpoch) - static_cast<long long>(other.m_daysSinceEpoch);
}

Date& Date::operator+=(int days)
{
	if (!m_valid)
		return *this;

	long long newDays = static_cast<long long>(m_daysSinceEpoch) + days;

	if (newDays < 0 || newDays > static_cast<long long>(MaxDays()))
	{
		m_valid = false;
		return *this;
	}
	m_daysSinceEpoch = static_cast<unsigned>(newDays);
	return *this;
}

Date& Date::operator-=(int days)
{
	return *this += (-days);
}

bool Date::operator==(const Date& other) const
{
	if (m_valid != other.m_valid)
		return false;
	if (!m_valid)
		return true;
	return m_daysSinceEpoch == other.m_daysSinceEpoch;
}

bool Date::operator!=(const Date& other) const
{
	return !(*this == other);
}

bool Date::operator<(const Date& other) const
{
	if (!m_valid || !other.m_valid)
		return false;
	return m_daysSinceEpoch < other.m_daysSinceEpoch;
}

bool Date::operator>(const Date& other) const
{
	return other < *this;
}

bool Date::operator<=(const Date& other) const
{
	return !(other < *this);
}

bool Date::operator>=(const Date& other) const
{
	return !(*this < other);
}

std::ostream& operator<<(std::ostream& os, const Date& date)
{
	if (!date.IsValid())
		return os << "INVALID";

	auto [day, month, year] = FromTimestamp(date.m_daysSinceEpoch);

	return os << std::setw(2) << std::setfill('0') << day << "."
			  << std::setw(2) << std::setfill('0') << static_cast<unsigned>(month) << "."
			  << std::setw(4) << std::setfill('0') << year;
}

std::istream& operator>>(std::istream& is, Date& date)
{
	unsigned day, month, year;
	char dot1, dot2;

	if (is >> day >> dot1 >> month >> dot2 >> year)
	{
		if (dot1 != '.' || dot2 != '.')
		{
			is.setstate(std::ios::failbit);
			date = Date();
			return is;
		}

		if (month < 1 || month > 12)
		{
			is.setstate(std::ios::failbit);
			date = Date();
			return is;
		}

		try
		{
			date = Date(day, static_cast<Month>(month), year);
		}
		catch (...)
		{
			is.setstate(std::ios::failbit);
			date = Date();
		}
	}
	else
	{
		is.clear();
		if (std::string token; is >> token && token == "INVALID")
		{
			date = Date();
			date.m_valid = false;
		}
		else
			is.setstate(std::ios::failbit);
	}
	return is;
}