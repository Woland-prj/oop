#ifndef BINARY_EXPRESSION_H
#define BINARY_EXPRESSION_H

#include "IExpression.h"
#include "types/Operation.h"
#include <memory>

class BinaryExpression : public IExpression
{
public:
	BinaryExpression(std::unique_ptr<IExpression> left,
		std::unique_ptr<IExpression> right,
		Operation op);
	void Accept(IExpressionVisitor& visitor) const override;

	const IExpression& GetLeft() const;
	const IExpression& GetRight() const;
	Operation GetOperation() const;

private:
	std::unique_ptr<IExpression> m_left;
	std::unique_ptr<IExpression> m_right;
	Operation m_op;
};

#endif // BINARY_EXPRESSION
