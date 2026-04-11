#include "CommandHandlers.h"
#include "OutputFormatter.h"
#include "ast/BinaryExpression.h"
#include "ast/IdentifierExpression.h"
#include "context/Context.h"
#include "parser/Parser.h"
#include <algorithm>
#include <charconv>
#include <iostream>
#include <vector>

namespace CommandHandlers
{

CommandResult HandleVar(std::istringstream& args, Context& ctx)
{
	std::string name;
	args >> name;
	args >> std::ws;
	if (!args.eof() || !Parser::IsValidIdentifier(name))
	{
		return CommandResult::InvalidUsage;
	}
	return ctx.AddVariable(name);
}

CommandResult HandleLet(std::istringstream& args, Context& ctx)
{
	std::string content;
	std::getline(args, content);
	auto eqPos = content.find('=');
	if (eqPos == std::string::npos)
		return CommandResult::InvalidUsage;

	std::string name = Parser::TrimSpaces(content.substr(0, eqPos));
	std::string rhs = Parser::TrimSpaces(content.substr(eqPos + 1));

	if (!Parser::IsValidIdentifier(name))
		return CommandResult::InvalidUsage;
	if (ctx.HasFunction(name))
		return CommandResult::InvalidUsage;

	if (!ctx.HasIdentifier(name))
		ctx.AddVariable(name);

	double valueToAssign = std::nan("");
	bool success = false;

	double numVal = 0.0;
	auto [ptr, ec] = std::from_chars(rhs.data(), rhs.data() + rhs.size(), numVal);
	if (ec == std::errc() && ptr == rhs.data() + rhs.size())
	{
		valueToAssign = numVal;
		success = true;
	}
	else if (Parser::IsValidIdentifier(rhs))
	{
		if (!ctx.HasIdentifier(rhs))
			return CommandResult::NameNotFound;
		auto resolved = ctx.ResolveValue(rhs);
		valueToAssign = resolved.value_or(std::nan(""));
		success = true;
	}
	else
		return CommandResult::InvalidUsage;

	if (success)
	{
		ctx.SetVariableValue(name, valueToAssign);
		return CommandResult::Success;
	}
	return CommandResult::InvalidUsage;
}

CommandResult HandleFn(std::istringstream& args, Context& ctx)
{
	std::string content;
	std::getline(args, content);
	auto eqPos = content.find('=');
	if (eqPos == std::string::npos)
		return CommandResult::InvalidUsage;

	std::string name = Parser::TrimSpaces(content.substr(0, eqPos));
	std::string rhs = Parser::TrimSpaces(content.substr(eqPos + 1));

	if (!Parser::IsValidIdentifier(name))
		return CommandResult::InvalidUsage;
	if (ctx.HasIdentifier(name))
		return CommandResult::NameExists;

	char op = '\0';
	std::string left, right;
	for (char c : { '+', '-', '*', '/' })
	{
		size_t pos = rhs.find(c);
		if (pos != std::string::npos)
		{
			op = c;
			left = Parser::TrimSpaces(rhs.substr(0, pos));
			right = Parser::TrimSpaces(rhs.substr(pos + 1));
			break;
		}
	}

	std::unique_ptr<IExpression> expr;
	if (op == '\0')
	{
		if (!Parser::IsValidIdentifier(rhs))
			return CommandResult::InvalidUsage;
		expr = std::make_unique<IdentifierExpression>(rhs);
	}
	else
	{
		if (!Parser::IsValidIdentifier(left) || !Parser::IsValidIdentifier(right))
			return CommandResult::InvalidUsage;
		auto lExpr = std::make_unique<IdentifierExpression>(left);
		auto rExpr = std::make_unique<IdentifierExpression>(right);
		Operation oper = Operation::Add;
		switch (op)
		{
		case '+':
			oper = Operation::Add;
			break;
		case '-':
			oper = Operation::Sub;
			break;
		case '*':
			oper = Operation::Mul;
			break;
		case '/':
			oper = Operation::Div;
			break;
		}
		expr = std::make_unique<BinaryExpression>(std::move(lExpr), std::move(rExpr), oper);
	}

	return ctx.AddFunction(name, std::move(expr));
}

CommandResult HandlePrint(std::istringstream& args, Context& ctx)
{
	std::string name;
	args >> name;
	args >> std::ws;
	if (!args.eof() || !Parser::IsValidIdentifier(name))
		return CommandResult::InvalidUsage;
	if (!ctx.HasIdentifier(name))
		return CommandResult::NameNotFound;

	auto val = ctx.ResolveValue(name);
	std::cout << OutputFormatter::FormatValue(val) << "\n";
	return CommandResult::Success;
}

CommandResult HandlePrintVars(std::istringstream& args, Context& ctx)
{
	std::string dummy;
	if (args >> dummy)
		return CommandResult::InvalidUsage;

	auto& vars = ctx.GetVariables();
	if (vars.empty())
		return CommandResult::Success;

	std::vector<std::string> names;
	names.reserve(vars.size());
	for (const auto& [n, _] : vars)
		names.push_back(n);
	std::sort(names.begin(), names.end());

	for (const auto& n : names)
		std::cout << n << ":" << OutputFormatter::FormatValue(vars.at(n).GetValue()) << "\n";
	return CommandResult::Success;
}

CommandResult HandlePrintFns(std::istringstream& args, Context& ctx)
{
	std::string dummy;
	if (args >> dummy)
		return CommandResult::InvalidUsage;

	auto& funcs = ctx.GetFunctions();
	if (funcs.empty())
		return CommandResult::Success;

	std::vector<std::string> names;
	names.reserve(funcs.size());
	for (const auto& [n, _] : funcs)
		names.push_back(n);
	std::sort(names.begin(), names.end());

	for (const auto& n : names)
	{
		auto val = ctx.ResolveValue(n);
		std::cout << n << ":" << OutputFormatter::FormatValue(val) << "\n";
	}
	return CommandResult::Success;
}

} // namespace CommandHandlers
