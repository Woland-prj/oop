#include "identifiers/Variable.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Variable initializes with undefined value")
{
	Variable var("x");
	REQUIRE_FALSE(var.GetValue().has_value());
	REQUIRE(var.GetName() == "x");
}

TEST_CASE("Variable stores and retrieves value")
{
	Variable var("temp");
	var.SetValue(36.6);
	REQUIRE(var.GetValue().has_value());
	REQUIRE(var.GetValue().value() == 36.6);
}

TEST_CASE("Variable clears value to undefined state")
{
	Variable var("y");
	var.SetValue(10.0);
	var.ClearValue();
	REQUIRE_FALSE(var.GetValue().has_value());
}
