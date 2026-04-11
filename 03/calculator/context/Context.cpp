#include "Context.h"
#include "ast/DependencyVisitor.h"
#include "ast/EvaluateVisitor.h"

CommandResult Context::AddVariable(const std::string& name)
{
	if (HasIdentifier(name))
		return CommandResult::NameExists;
	m_variables.emplace(name, name);
	m_versions[name] = 0;
	return CommandResult::Success;
}

CommandResult Context::SetVariableValue(const std::string& name, double value)
{
	if (!HasVariable(name))
		return CommandResult::NameNotFound;
	m_variables.at(name).SetValue(value);
	IncrementVersion(name);
	return CommandResult::Success;
}

CommandResult Context::AddFunction(const std::string& name, std::unique_ptr<IExpression> expr)
{
	if (HasIdentifier(name))
		return CommandResult::NameExists;

	DependencyVisitor depVisitor;
	auto deps = depVisitor.ExtractDependencies(*expr);
	std::vector<std::string> depVec(deps.begin(), deps.end());

	if (WouldCreateCycle(name, depVec))
		return CommandResult::InvalidUsage;

	auto funcNode = std::make_unique<FunctionNode>(name, std::move(expr), std::move(depVec));
	m_functions[name] = std::move(funcNode);
	m_versions[name] = 0;

	for (const auto& dep : m_functions[name]->GetDependencies())
		m_dependencyGraph.AddDependency(name, dep);
	return CommandResult::Success;
}

std::optional<double> Context::ResolveValue(const std::string& identifier)
{
	if (HasVariable(identifier))
		return m_variables.at(identifier).GetValue();
	if (HasFunction(identifier))
		return EvaluateFunction(identifier);
	return std::nullopt;
}

bool Context::HasIdentifier(const std::string& name) const
{
	return HasVariable(name) || HasFunction(name);
}

bool Context::HasFunction(const std::string& name) const
{
	return m_functions.count(name);
}

bool Context::HasVariable(const std::string& name) const
{
	return m_variables.count(name);
}

size_t Context::GetVersion(const std::string& identifier) const
{
	auto it = m_versions.find(identifier);
	return (it != m_versions.end()) ? it->second : 0;
}

bool Context::WouldCreateCycle(const std::string& funcName, const std::vector<std::string>& deps) const
{
	std::vector<std::pair<std::string, std::string>> tempEdges;
	for (const auto& dep : deps)
	{
		m_dependencyGraph.AddDependency(funcName, dep);
		tempEdges.emplace_back(funcName, dep);
	}

	bool hasCycle = m_dependencyGraph.HasCycle(funcName);

	for (const auto& edge : tempEdges)
		m_dependencyGraph.RemoveDependency(edge.first, edge.second);
	return hasCycle;
}

void Context::IncrementVersion(const std::string& identifier)
{
	++m_versions[identifier];
}

std::optional<double> Context::EvaluateFunction(std::string funcName)
{
	auto it = m_functions.find(funcName);
	if (it == m_functions.end())
		return std::nullopt;

	auto& funcNode = it->second;
	auto result = funcNode->Evaluate(*this);

	if (result.has_value())
		IncrementVersion(funcName);
	return result;
}

const std::unordered_map<std::string, Variable>& Context::GetVariables() const
{
	return m_variables;
}

const std::unordered_map<std::string, std::unique_ptr<FunctionNode>>& Context::GetFunctions() const
{
	return m_functions;
}
