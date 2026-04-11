#ifndef NUMBER_EXPRESSION_H
#define NUMBER_EXPRESSION_H

#include "IExpression.h"

class NumberExpression : public IExpression
{
public:
	explicit NumberExpression(double value);
	void Accept(IExpressionVisitor& visitor) const override;
	double GetValue() const;

private:
	double m_value;
};

#endif // NUMBER_EXPRESSION_H
