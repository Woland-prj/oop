#include "identifiers/DependencyGraph.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("DependencyGraph tracks reverse dependencies")
{
	DependencyGraph graph;
	graph.AddDependency("func", "varX");
	graph.AddDependency("func", "varY");

	auto dependentsOfX = graph.GetDependents("varX");
	REQUIRE(dependentsOfX.size() == 1);
	REQUIRE(dependentsOfX[0] == "func");
}

TEST_CASE("DependencyGraph detects cycles")
{
	DependencyGraph graph;
	graph.AddDependency("A", "B");
	graph.AddDependency("B", "C");
	REQUIRE_FALSE(graph.HasCycle("A"));

	graph.AddDependency("C", "A");
	REQUIRE(graph.HasCycle("A"));
}

TEST_CASE("Topological sort returns evaluation order")
{
	DependencyGraph graph;
	graph.AddDependency("C", "B");
	graph.AddDependency("B", "A");

	auto order = graph.TopologicalSortFrom("C");
	REQUIRE(order.size() == 3);
	REQUIRE(order[0] == "A");
	REQUIRE(order[1] == "B");
	REQUIRE(order[2] == "C");
}
