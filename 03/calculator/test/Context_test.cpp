#include "ast/IdentifierExpression.h"
#include "ast/NumberExpression.h"
#include "context/Context.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Context manages variables and tracks versions")
{
	Context ctx;
	REQUIRE(ctx.AddVariable("x") == CommandResult::Success);
	REQUIRE(ctx.SetVariableValue("x", 5.0) == CommandResult::Success);
	REQUIRE(ctx.GetVersion("x") == 1);
	REQUIRE(ctx.ResolveValue("x").value() == 5.0);
}

TEST_CASE("Context resolves function values lazily")
{
	Context ctx;
	ctx.AddVariable("a");
	ctx.SetVariableValue("a", 10.0);

	auto expr = std::make_unique<IdentifierExpression>("a");
	REQUIRE(ctx.AddFunction("fa", std::move(expr)) == CommandResult::Success);

	auto val = ctx.ResolveValue("fa");
	REQUIRE(val.has_value());
	REQUIRE(val.value() == 10.0);
}

TEST_CASE("Context prevents duplicate identifiers")
{
	Context ctx;
	ctx.AddVariable("x");
	REQUIRE(ctx.AddVariable("x") == CommandResult::NameExists);

	auto expr = std::make_unique<NumberExpression>(1.0);
	REQUIRE(ctx.AddFunction("x", std::move(expr)) == CommandResult::NameExists);
}
