#ifndef I_EXPRESSION_H
#define I_EXPRESSION_H

class IExpressionVisitor;

class IExpression
{
public:
	virtual ~IExpression() = default;
	virtual void Accept(IExpressionVisitor& visitor) const = 0;
};

#endif // I_EXPRESSION_H
