#include "DependencyGraph.h"

bool DependencyGraph::AddDependency(const std::string& from, const std::string& to)
{
	m_edges[from].push_back(to);
	m_reverseEdges[to].push_back(from);
	return true;
}

bool DependencyGraph::HasCycle(const std::string& start) const
{
	std::set<std::string> visited;
	std::set<std::string> recStack;
	return HasCycleDfs(start, visited, recStack);
}

bool DependencyGraph::HasCycleDfs(const std::string& node, std::set<std::string>& visited, std::set<std::string>& recStack) const
{
	visited.insert(node);
	recStack.insert(node);

	auto it = m_edges.find(node);
	if (it != m_edges.end())
	{
		for (const auto& dep : it->second)
		{
			if (recStack.count(dep))
				return true;
			if (!visited.count(dep))
			{
				if (HasCycleDfs(dep, visited, recStack))
					return true;
			}
		}
	}
	recStack.erase(node);
	return false;
}

std::vector<std::string> DependencyGraph::TopologicalSortFrom(const std::string& start) const
{
	std::vector<std::string> result;
	std::set<std::string> visited;
	TopoDfs(start, visited, result);
	return result;
}

void DependencyGraph::TopoDfs(const std::string& node, std::set<std::string>& visited, std::vector<std::string>& result) const
{
	if (visited.count(node))
		return;
	visited.insert(node);

	auto it = m_edges.find(node);
	if (it != m_edges.end())
	{
		for (const auto& dep : it->second)
			TopoDfs(dep, visited, result);
	}
	result.push_back(node);
}

const std::vector<std::string>& DependencyGraph::GetDependents(const std::string& identifier) const
{
	auto it = m_reverseEdges.find(identifier);
	if (it != m_reverseEdges.end())
		return it->second;
	static const std::vector<std::string> emptyDeps;
	return emptyDeps;
}

void DependencyGraph::Clear()
{
	m_edges.clear();
	m_reverseEdges.clear();
}

void DependencyGraph::RemoveDependency(const std::string& from, const std::string& to)
{
	auto& vec = m_edges[from];
	vec.erase(std::remove(vec.begin(), vec.end(), to), vec.end());
	auto& revVec = m_reverseEdges[to];
	revVec.erase(std::remove(revVec.begin(), revVec.end(), from), revVec.end());
}
