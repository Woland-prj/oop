#ifndef EVALUATE_VISITOR_H
#define EVALUATE_VISITOR_H

#include "IExpressionVisitor.h"
#include <optional>

class Context;

class EvaluateVisitor : public IExpressionVisitor
{
public:
	explicit EvaluateVisitor(Context& context);

	void VisitNumber(const NumberExpression& expr) override;
	void VisitIdentifier(const IdentifierExpression& expr) override;
	void VisitBinary(const BinaryExpression& expr) override;

	std::optional<double> GetResult() const;
	bool HasValue() const;

private:
	Context& m_context;
	std::optional<double> m_result;
};

#endif // EVALUATE_VISITOR_H
