#include "FunctionNode.h"
#include "ast/EvaluateVisitor.h"
#include "context/Context.h"
#include <iostream>

FunctionNode::FunctionNode(std::string name, std::unique_ptr<IExpression> expression, std::vector<std::string> dependencies)
	: m_name(std::move(name))
	, m_expression(std::move(expression))
	, m_dependencies(std::move(dependencies))
{
}

std::optional<double> FunctionNode::Evaluate(Context& context)
{
	if (IsCacheValid(context))
	{
		std::cout << "cache valid";
		return m_cachedValue;
	}

	EvaluateVisitor visitor(context);
	m_expression->Accept(visitor);
	std::optional<double> result = visitor.GetResult();

	if (result.has_value())
	{
		std::map<std::string, size_t> currentVersions;
		for (const auto& dep : m_dependencies)
			currentVersions[dep] = context.GetVersion(dep);
		m_cachedValue = result;
		m_depVersions = std::move(currentVersions);
	}
	else
	{
		m_cachedValue = std::nullopt;
		m_depVersions.clear();
	}
	return m_cachedValue;
}

bool FunctionNode::IsCacheValid(const Context& context) const
{
	if (!m_cachedValue.has_value())
		return false;
	for (const auto& dep : m_dependencies)
	{
		auto it = m_depVersions.find(dep);
		if (it == m_depVersions.end() || context.GetVersion(dep) != it->second)
			return false;
	}
	return true;
}

void FunctionNode::UpdateCache(double value, const std::map<std::string, size_t>& versions)
{
	m_cachedValue = value;
	m_depVersions = versions;
}

void FunctionNode::InvalidateCache()
{
	m_cachedValue = std::nullopt;
	m_depVersions.clear();
}

const std::string& FunctionNode::GetName() const
{
	return m_name;
}

const std::vector<std::string>& FunctionNode::GetDependencies() const
{
	return m_dependencies;
}
