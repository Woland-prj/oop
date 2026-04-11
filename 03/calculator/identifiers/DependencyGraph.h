#ifndef DEPENDENCY_GRAPH_H
#define DEPENDENCY_GRAPH_H

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

class DependencyGraph
{
public:
	DependencyGraph() = default;

	bool AddDependency(const std::string& from, const std::string& to);
	bool HasCycle(const std::string& start) const;
	std::vector<std::string> TopologicalSortFrom(const std::string& start) const;
	const std::vector<std::string>& GetDependents(const std::string& identifier) const;
	void RemoveDependency(const std::string& from, const std::string& to);
	void Clear();

private:
	std::unordered_map<std::string, std::vector<std::string>> m_edges;
	std::unordered_map<std::string, std::vector<std::string>> m_reverseEdges;

	bool HasCycleDfs(const std::string& node, std::set<std::string>& visited, std::set<std::string>& recStack) const;
	void TopoDfs(const std::string& node, std::set<std::string>& visited, std::vector<std::string>& result) const;
};

#endif // DEPENDENCY_GRAPH_H
