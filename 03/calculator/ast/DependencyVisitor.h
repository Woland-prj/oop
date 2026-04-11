#ifndef DEPENDENCY_VISITOR_H
#define DEPENDENCY_VISITOR_H

#include "IExpression.h"
#include "IExpressionVisitor.h"
#include <set>
#include <string>

class DependencyVisitor : public IExpressionVisitor
{
public:
	DependencyVisitor() = default;

	void VisitNumber(const NumberExpression& expr) override;
	void VisitIdentifier(const IdentifierExpression& expr) override;
	void VisitBinary(const BinaryExpression& expr) override;

	std::set<std::string> ExtractDependencies(const IExpression& root);

private:
	std::set<std::string> m_dependencies;
};

#endif // DEPENDENCY_VISITOR_H
