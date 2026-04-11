#include "EvaluateVisitor.h"
#include "BinaryExpression.h"
#include "IdentifierExpression.h"
#include "NumberExpression.h"
#include "context/Context.h"
#include "types/Operation.h"

EvaluateVisitor::EvaluateVisitor(Context& context)
	: m_context(context)
	, m_result(std::nullopt)
{
}

void EvaluateVisitor::VisitNumber(const NumberExpression& expr)
{
	m_result = expr.GetValue();
}

void EvaluateVisitor::VisitIdentifier(const IdentifierExpression& expr)
{
	m_result = m_context.ResolveValue(expr.GetName());
}

void EvaluateVisitor::VisitBinary(const BinaryExpression& expr)
{
	expr.GetLeft().Accept(*this);
	std::optional<double> leftVal = m_result;

	expr.GetRight().Accept(*this);
	std::optional<double> rightVal = m_result;

	if (leftVal.has_value() && rightVal.has_value())
		m_result = ApplyOperation(expr.GetOperation(), leftVal.value(), rightVal.value());
	else
		m_result = std::nullopt;
}

std::optional<double> EvaluateVisitor::GetResult() const
{
	return m_result;
}

bool EvaluateVisitor::HasValue() const
{
	return m_result.has_value();
}
