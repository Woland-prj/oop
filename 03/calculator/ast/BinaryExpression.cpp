#include "BinaryExpression.h"
#include "IExpressionVisitor.h"

BinaryExpression::BinaryExpression(
	std::unique_ptr<IExpression> left,
	std::unique_ptr<IExpression> right,
	Operation op)
	: m_left(std::move(left))
	, m_right(std::move(right))
	, m_op(op)
{
}

void BinaryExpression::Accept(IExpressionVisitor& visitor) const
{
	visitor.VisitBinary(*this);
}

const IExpression& BinaryExpression::GetLeft() const
{
	return *m_left;
}

const IExpression& BinaryExpression::GetRight() const
{
	return *m_right;
}

Operation BinaryExpression::GetOperation() const
{
	return m_op;
}
