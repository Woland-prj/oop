#include "Variable.h"

Variable::Variable(std::string name)
	: m_name(std::move(name))
	, m_value(std::nullopt)
{
}

std::optional<double> Variable::GetValue() const
{
	return m_value;
}

void Variable::SetValue(double value)
{
	m_value = value;
}

void Variable::ClearValue()
{
	m_value = std::nullopt;
}

const std::string& Variable::GetName() const
{
	return m_name;
}
