#ifndef CONTEXT_H
#define CONTEXT_H

#include "ast/IExpression.h"
#include "identifiers/DependencyGraph.h"
#include "identifiers/FunctionNode.h"
#include "identifiers/Variable.h"
#include "types/CommandResult.h"
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class Context
{
public:
	Context() = default;
	Context(const Context&) = delete;
	Context& operator=(const Context&) = delete;

	CommandResult AddVariable(const std::string& name);
	CommandResult SetVariableValue(const std::string& name, double value);
	CommandResult AddFunction(const std::string& name, std::unique_ptr<IExpression> expr);
	std::optional<double> ResolveValue(const std::string& identifier);

	bool HasIdentifier(const std::string& name) const;
	bool HasFunction(const std::string& name) const;
	bool HasVariable(const std::string& name) const;
	size_t GetVersion(const std::string& identifier) const;
	bool WouldCreateCycle(const std::string& funcName, const std::vector<std::string>& deps) const;

	const std::unordered_map<std::string, Variable>& GetVariables() const;
	const std::unordered_map<std::string, std::unique_ptr<FunctionNode>>& GetFunctions() const;

private:
	std::unordered_map<std::string, Variable> m_variables;
	std::unordered_map<std::string, std::unique_ptr<FunctionNode>> m_functions;
	mutable DependencyGraph m_dependencyGraph;
	std::unordered_map<std::string, size_t> m_versions;

	void IncrementVersion(const std::string& identifier);
	std::optional<double> EvaluateFunction(std::string funcName);
};

#endif // CONTEXT_H
