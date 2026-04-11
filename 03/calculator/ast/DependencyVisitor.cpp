#include "DependencyVisitor.h"
#include "BinaryExpression.h"
#include "IdentifierExpression.h"
#include "NumberExpression.h"

void DependencyVisitor::VisitNumber(const NumberExpression&)
{
}

void DependencyVisitor::VisitIdentifier(const IdentifierExpression& expr)
{
	m_dependencies.insert(expr.GetName());
}

void DependencyVisitor::VisitBinary(const BinaryExpression& expr)
{
	expr.GetLeft().Accept(*this);
	expr.GetRight().Accept(*this);
}

std::set<std::string> DependencyVisitor::ExtractDependencies(const IExpression& root)
{
	m_dependencies.clear();
	root.Accept(*this);
	return m_dependencies;
}
