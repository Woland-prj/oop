#include "IdentifierExpression.h"
#include "IExpressionVisitor.h"

IdentifierExpression::IdentifierExpression(std::string name)
	: m_name(std::move(name))
{
}

void IdentifierExpression::Accept(IExpressionVisitor& visitor) const
{
	visitor.VisitIdentifier(*this);
}

const std::string& IdentifierExpression::GetName() const
{
	return m_name;
}
