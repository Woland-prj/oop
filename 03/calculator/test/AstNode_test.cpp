#include "../ast/BinaryExpression.h"
#include "../ast/IExpressionVisitor.h"
#include "../ast/IdentifierExpression.h"
#include "../ast/NumberExpression.h"
#include "../types/Operation.h"
#include <catch2/catch_test_macros.hpp>

class MockExpressionVisitor : public IExpressionVisitor
{
public:
	bool m_numberVisited = false;
	bool m_identifierVisited = false;
	bool m_binaryVisited = false;

	double m_capturedNumberValue = 0.0;
	std::string m_capturedIdentifierName;

	void VisitNumber(const NumberExpression& expr) override
	{
		m_numberVisited = true;
		m_capturedNumberValue = expr.GetValue();
	}

	void VisitIdentifier(const IdentifierExpression& expr) override
	{
		m_identifierVisited = true;
		m_capturedIdentifierName = expr.GetName();
	}

	void VisitBinary(const BinaryExpression& expr) override
	{
		m_binaryVisited = true;
	}
};

TEST_CASE("NumberExpression stores and exposes value")
{
	NumberExpression expr(42.5);
	REQUIRE(expr.GetValue() == 42.5);
}

TEST_CASE("NumberExpression Accept calls VisitNumber")
{
	NumberExpression expr(3.14);
	MockExpressionVisitor visitor;

	expr.Accept(visitor);

	REQUIRE(visitor.m_numberVisited);
	REQUIRE(visitor.m_capturedNumberValue == 3.14);
}

TEST_CASE("IdentifierExpression stores name")
{
	IdentifierExpression expr("myVar");
	REQUIRE(expr.GetName() == "myVar");
}

TEST_CASE("IdentifierExpression Accept calls VisitIdentifier")
{
	IdentifierExpression expr("funcName");
	MockExpressionVisitor visitor;

	expr.Accept(visitor);

	REQUIRE(visitor.m_identifierVisited);
	REQUIRE(visitor.m_capturedIdentifierName == "funcName");
}

TEST_CASE("BinaryExpression owns left and right expressions")
{
	auto left = std::make_unique<NumberExpression>(10.0);
	auto right = std::make_unique<IdentifierExpression>("x");

	BinaryExpression expr(std::move(left), std::move(right), Operation::Add);

	REQUIRE(dynamic_cast<const NumberExpression*>(&expr.GetLeft()) != nullptr);
	REQUIRE(dynamic_cast<const IdentifierExpression*>(&expr.GetRight()) != nullptr);
	REQUIRE(expr.GetOperation() == Operation::Add);
}

TEST_CASE("BinaryExpression Accept calls VisitBinary")
{
	auto left = std::make_unique<NumberExpression>(1.0);
	auto right = std::make_unique<NumberExpression>(2.0);
	BinaryExpression expr(std::move(left), std::move(right), Operation::Mul);

	MockExpressionVisitor visitor;
	expr.Accept(visitor);

	REQUIRE(visitor.m_binaryVisited);
}
