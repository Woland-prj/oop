#ifndef IDENTIFIER_EXPRESSION_H
#define IDENTIFIER_EXPRESSION_H

#include "IExpression.h"
#include <string>

class IdentifierExpression : public IExpression
{
public:
	explicit IdentifierExpression(std::string name);
	void Accept(IExpressionVisitor& visitor) const override;
	const std::string& GetName() const;

private:
	std::string m_name;
};

#endif // IDENTIFIER_EXPRESSION_H
