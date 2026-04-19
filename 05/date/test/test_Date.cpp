#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../Date.h"
#include <sstream>

// Конструкторы
TEST_CASE("Default constructor = 01.01.1970", "[ctor]")
{
    Date d;
    REQUIRE(d.GetDay() == 1);
    REQUIRE(d.GetMonth() == Month::JANUARY);
    REQUIRE(d.GetYear() == 1970);
}

TEST_CASE("Construct from valid date", "[ctor]")
{
    Date d(15, Month::MARCH, 2020);
    REQUIRE(d.GetDay() == 15);
    REQUIRE(d.GetMonth() == Month::MARCH);
    REQUIRE(d.GetYear() == 2020);
}

TEST_CASE("Construct from timestamp", "[ctor]")
{
    Date d(0);
    REQUIRE(d.GetDay() == 1);
    REQUIRE(d.GetMonth() == Month::JANUARY);
    REQUIRE(d.GetYear() == 1970);

    Date d2(31); // 01.02.1970
    REQUIRE(d2.GetDay() == 1);
    REQUIRE(d2.GetMonth() == Month::FEBRUARY);
}

// Валидация
TEST_CASE("Invalid dates throw", "[validation]")
{
    REQUIRE_THROWS(Date(32, Month::JANUARY, 2020));
    REQUIRE_THROWS(Date(29, Month::FEBRUARY, 2019));
    REQUIRE_THROWS(Date(1, static_cast<Month>(13), 2020));
    REQUIRE_THROWS(Date(1, Month::JANUARY, 1969));
}

// Високосный
TEST_CASE("Leap year handling", "[leap]")
{
    Date d(29, Month::FEBRUARY, 2020);
    REQUIRE(d.GetDay() == 29);

    REQUIRE_THROWS(Date(29, Month::FEBRUARY, 1900)); // не leap
    REQUIRE_NOTHROW(Date(29, Month::FEBRUARY, 2000)); // leap
}

// День недели
TEST_CASE("Weekday correctness", "[weekday]")
{
    Date d; // 01.01.1970 — Thursday
    REQUIRE(d.GetWeekDay() == WeekDay::THURSDAY);

    Date d2(2, Month::JANUARY, 1970);
    REQUIRE(d2.GetWeekDay() == WeekDay::FRIDAY);

    Date d3(3, Month::JANUARY, 1970);
    REQUIRE(d3.GetWeekDay() == WeekDay::SATURDAY);
}

// Операторы
TEST_CASE("Increment operator", "[inc]")
{
    Date d(31, Month::JANUARY, 1970);
    ++d;
    REQUIRE(d.GetDay() == 1);
    REQUIRE(d.GetMonth() == Month::FEBRUARY);
}

TEST_CASE("Decrement operator", "[dec]")
{
    Date d(1, Month::MARCH, 1970);
    --d;
    REQUIRE(d.GetDay() == 28);
    REQUIRE(d.GetMonth() == Month::FEBRUARY);
}

TEST_CASE("Add days", "[add]")
{
    const Date d(28, Month::FEBRUARY, 2010);
    const Date r = d + 3;

    REQUIRE(r.GetDay() == 3);
    REQUIRE(r.GetMonth() == Month::MARCH);
}

TEST_CASE("Subtract days", "[sub]")
{
    const Date d(1, Month::JANUARY, 2010);
    const Date r = d - 2;

    REQUIRE(r.GetDay() == 30);
    REQUIRE(r.GetMonth() == Month::DECEMBER);
    REQUIRE(r.GetYear() == 2009);
}

TEST_CASE("Date difference", "[diff]")
{
    const Date d1(1, Month::JANUARY, 2010);
    const Date d2(30, Month::DECEMBER, 2009);

    REQUIRE(d1 - d2 == 2);
    REQUIRE(d2 - d1 == -2);
}

TEST_CASE("Plus equals", "[plus_eq]")
{
    Date d(1, Month::JANUARY, 2020);
    d += 10;

    REQUIRE(d.GetDay() == 11);
}

TEST_CASE("Minus equals", "[minus_eq]")
{
    Date d(10, Month::JANUARY, 2020);
    d -= 5;

    REQUIRE(d.GetDay() == 5);
}

// Сравнения
TEST_CASE("Comparison operators", "[compare]")
{
    Date d1(1, Month::JANUARY, 2020);
    Date d2(2, Month::JANUARY, 2020);

    REQUIRE(d1 < d2);
    REQUIRE(d2 > d1);
    REQUIRE(d1 != d2);

    Date d3(1, Month::JANUARY, 2020);
    REQUIRE(d1 == d3);
}

// Граничные случаи
TEST_CASE("Upper bound overflow", "[bounds]")
{
    Date d(31, Month::DECEMBER, 9999);
    ++d;
    REQUIRE_FALSE(d.IsValid());
}

TEST_CASE("Lower bound overflow", "[bounds]")
{
    Date d(1, Month::JANUARY, 1970);
    --d;
    REQUIRE_FALSE(d.IsValid());
}

// IO
TEST_CASE("Output format", "[io]")
{
    Date d(5, Month::MARCH, 2020);

    std::stringstream ss;
    ss << d;

    REQUIRE(ss.str() == "05.03.2020");
}

TEST_CASE("Input format", "[io]")
{
    std::stringstream ss("15.04.2020");

    Date d;
    ss >> d;

    REQUIRE(d.GetDay() == 15);
    REQUIRE(d.GetMonth() == Month::APRIL);
    REQUIRE(d.GetYear() == 2020);
}

TEST_CASE("Invalid input", "[io]")
{
    std::stringstream ss("INVALID");

    Date d;
    ss >> d;

    REQUIRE_FALSE(d.IsValid());
}