#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../ExpressionParser.hpp"

#include <sstream>
#include <string>

static int64_t Eval(const std::string& expr)
{
	std::istringstream input(expr);
	return EvalPrefixExpression(input);
}

// valid cases
TEST_CASE("Single argument addition", "[EvalPrefixExpression]")
{
	REQUIRE(Eval("(+ 7)") == 7);
}

TEST_CASE("Single argument multiplication", "[EvalPrefixExpression]")
{
	REQUIRE(Eval("(* 8)") == 8);
}

TEST_CASE("Simple addition", "[EvalPrefixExpression]")
{
	REQUIRE(Eval("(+ 2 3)") == 5);
}

TEST_CASE("Addition with multiple arguments", "[EvalPrefixExpression]")
{
	REQUIRE(Eval("(+ 2 3 4)") == 9);
}

TEST_CASE("Simple multiplication", "[EvalPrefixExpression]")
{
	REQUIRE(Eval("(* 2 4)") == 8);
}

TEST_CASE("Multiplication with multiple arguments", "[EvalPrefixExpression]")
{
	REQUIRE(Eval("(* 2 3 4)") == 24);
}

TEST_CASE("Nested expressions 1", "[EvalPrefixExpression]")
{
	REQUIRE(Eval("(+ (* 2 3) (* 3 4))") == 18);
}

TEST_CASE("Nested expressions 2", "[EvalPrefixExpression]")
{
	REQUIRE(Eval("(* (+ 1 2) (+ 3 1))") == 12);
}

TEST_CASE("Complex nested expression", "[EvalPrefixExpression]")
{
	REQUIRE(Eval("(+ 5 (* 2 3 2) (+ 5 (+ 2 5) (* 2 2)))") == 33);
}

TEST_CASE("Negative numbers", "[EvalPrefixExpression]")
{
	REQUIRE(Eval("(+ -2 3)") == 1);
}

TEST_CASE("Multiplication with negative numbers", "[EvalPrefixExpression]")
{
	REQUIRE(Eval("(* -2 3)") == -6);
}

TEST_CASE("Nested negative numbers", "[EvalPrefixExpression]")
{
	REQUIRE(Eval("(+ (* -2 3) 10)") == 4);
}

// errors
TEST_CASE("Empty input throws", "[EvalPrefixExpression]")
{
	std::istringstream input("");
	REQUIRE_THROWS_AS(EvalPrefixExpression(input), std::runtime_error);
}

TEST_CASE("Unexpected character throws", "[EvalPrefixExpression]")
{
	std::istringstream input("(+ 2 a)");
	REQUIRE_THROWS_AS(EvalPrefixExpression(input), std::runtime_error);
}

TEST_CASE("Unexpected operator throws", "[EvalPrefixExpression]")
{
	std::istringstream input("(- 2 3)");
	REQUIRE_THROWS_AS(EvalPrefixExpression(input), std::runtime_error);
}

TEST_CASE("Unmatched closing parenthesis throws", "[EvalPrefixExpression]")
{
	std::istringstream input("(+ 2 3))");
	REQUIRE_THROWS_AS(EvalPrefixExpression(input), std::runtime_error);
}

TEST_CASE("Unclosed expression throws", "[EvalPrefixExpression]")
{
	std::istringstream input("(+ 2 3");
	REQUIRE_THROWS_AS(EvalPrefixExpression(input), std::runtime_error);
}

TEST_CASE("Extra data after expression throws", "[EvalPrefixExpression]")
{
	std::istringstream input("(+ 2 3) garbage");
	REQUIRE_THROWS_AS(EvalPrefixExpression(input), std::runtime_error);
}

TEST_CASE("Number outside expression throws", "[EvalPrefixExpression]")
{
	std::istringstream input("5");
	REQUIRE_THROWS_AS(EvalPrefixExpression(input), std::runtime_error);
}

TEST_CASE("Invalid number format throws", "[EvalPrefixExpression]")
{
	std::istringstream input("(+ - 3)");
	REQUIRE_THROWS_AS(EvalPrefixExpression(input), std::runtime_error);
}
