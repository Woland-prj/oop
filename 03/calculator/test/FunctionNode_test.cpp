#include "ast/IdentifierExpression.h"
#include "identifiers/FunctionNode.h"
#include <catch2/catch_test_macros.hpp>
#include <unordered_map>

struct MockContextForFn
{
	std::unordered_map<std::string, std::optional<double>> m_values;
	std::unordered_map<std::string, size_t> m_versions;

	std::optional<double> GetValue(const std::string& id) const
	{
		auto it = m_values.find(id);
		return (it != m_values.end()) ? it->second : std::nullopt;
	}
	size_t GetVersion(const std::string& id) const
	{
		auto it = m_versions.find(id);
		return (it != m_versions.end()) ? it->second : 0;
	}
	void IncrementVersion(const std::string& id) { ++m_versions[id]; }
};

TEST_CASE("FunctionNode caches result and invalidates on dependency change")
{
	MockContextForFn ctx;
	ctx.m_values["x"] = 5.0;
	ctx.m_versions["x"] = 1;

	auto expr = std::make_unique<IdentifierExpression>("x");
	FunctionNode func("fx", std::move(expr), { "x" });

	auto& ctxRef = reinterpret_cast<Context&>(ctx);
	auto res1 = func.Evaluate(ctxRef);
	REQUIRE(res1.has_value());
	REQUIRE(res1.value() == 5.0);

	auto res2 = func.Evaluate(ctxRef);
	REQUIRE(res2.has_value());
	REQUIRE(res2.value() == 5.0);

	ctx.m_values["x"] = 10.0;
	ctx.m_versions["x"] = 2;

	auto res3 = func.Evaluate(ctxRef);
	REQUIRE(res3.has_value());
	REQUIRE(res3.value() == 10.0);
}
