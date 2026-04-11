#ifndef I_EXPRESSION_VISITOR_H
#define I_EXPRESSION_VISITOR_H

class NumberExpression;
class IdentifierExpression;
class BinaryExpression;

class IExpressionVisitor
{
public:
	virtual ~IExpressionVisitor() = default;

	virtual void VisitNumber(const NumberExpression& expr) = 0;
	virtual void VisitIdentifier(const IdentifierExpression& expr) = 0;
	virtual void VisitBinary(const BinaryExpression& expr) = 0;
};

#endif // I_EXPRESSION_VISITOR_H
