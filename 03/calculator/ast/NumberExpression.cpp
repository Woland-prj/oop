#include "NumberExpression.h"
#include "IExpressionVisitor.h"

NumberExpression::NumberExpression(double value)
	: m_value(value)
{
}

void NumberExpression::Accept(IExpressionVisitor& visitor) const
{
	visitor.VisitNumber(*this);
}

double NumberExpression::GetValue() const
{
	return m_value;
}
