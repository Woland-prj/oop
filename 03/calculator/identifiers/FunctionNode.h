#ifndef FUNCTION_NODE_H
#define FUNCTION_NODE_H

#include "ast/IExpression.h"
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class Context;

class FunctionNode
{
public:
	FunctionNode(std::string name, std::unique_ptr<IExpression> expression, std::vector<std::string> dependencies);
	FunctionNode(const FunctionNode&) = delete;
	FunctionNode& operator=(const FunctionNode&) = delete;

	std::optional<double> Evaluate(Context& context);
	bool IsCacheValid(const Context& context) const;
	void UpdateCache(double value, const std::map<std::string, size_t>& versions);
	void InvalidateCache();

	const std::string& GetName() const;
	const std::vector<std::string>& GetDependencies() const;

private:
	std::string m_name;
	std::unique_ptr<IExpression> m_expression;
	std::vector<std::string> m_dependencies;
	std::optional<double> m_cachedValue;
	std::map<std::string, size_t> m_depVersions;
};

#endif // FUNCTION_NODE_H
