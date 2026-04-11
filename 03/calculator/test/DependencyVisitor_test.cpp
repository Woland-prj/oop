#include "ast/BinaryExpression.h"
#include "ast/DependencyVisitor.h"
#include "ast/IdentifierExpression.h"
#include "ast/NumberExpression.h"
#include <catch2/catch_test_macros.hpp>
#include <set>

TEST_CASE("DependencyVisitor extracts no dependencies from number")
{
	NumberExpression expr(42.0);
	DependencyVisitor visitor;
	REQUIRE(visitor.ExtractDependencies(expr).empty());
}

TEST_CASE("DependencyVisitor extracts single identifier")
{
	IdentifierExpression expr("temperature");
	DependencyVisitor visitor;

	auto deps = visitor.ExtractDependencies(expr);
	REQUIRE(deps.size() == 1);
	REQUIRE(deps.count("temperature") == 1);
}

TEST_CASE("DependencyVisitor extracts and deduplicates identifiers from binary expression")
{
	auto left = std::make_unique<IdentifierExpression>("x");
	auto right = std::make_unique<IdentifierExpression>("x");
	BinaryExpression expr(std::move(left), std::move(right), Operation::Add);

	DependencyVisitor visitor;
	auto deps = visitor.ExtractDependencies(expr);

	REQUIRE(deps.size() == 1);
	REQUIRE(deps.count("x") == 1);
}

TEST_CASE("DependencyVisitor collects all unique identifiers in nested expression")
{
	// (a + b) * (b + c)
	auto leftLeft = std::make_unique<IdentifierExpression>("a");
	auto leftRight = std::make_unique<IdentifierExpression>("b");
	auto leftBin = std::make_unique<BinaryExpression>(std::move(leftLeft), std::move(leftRight), Operation::Add);

	auto rightLeft = std::make_unique<IdentifierExpression>("b");
	auto rightRight = std::make_unique<IdentifierExpression>("c");
	auto rightBin = std::make_unique<BinaryExpression>(std::move(rightLeft), std::move(rightRight), Operation::Add);

	BinaryExpression root(std::move(leftBin), std::move(rightBin), Operation::Mul);

	DependencyVisitor visitor;
	auto deps = visitor.ExtractDependencies(root);

	REQUIRE(deps == std::set<std::string>{ "a", "b", "c" });
}
