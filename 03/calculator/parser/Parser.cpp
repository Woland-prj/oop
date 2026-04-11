#include "Parser.h"
#include "ast/BinaryExpression.h"
#include "ast/IdentifierExpression.h"
#include "ast/NumberExpression.h"
#include "types/Operation.h"
#include <algorithm>
#include <cctype>
#include <charconv>
#include <vector>

std::string Parser::TrimSpaces(const std::string& str)
{
	auto start = str.find_first_not_of(" \t\r\n");
	if (start == std::string::npos)
		return "";
	auto end = str.find_last_not_of(" \t\r\n");
	return str.substr(start, end - start + 1);
}

std::optional<std::string> Parser::ParseCommandName(const std::string& line)
{
	std::string trimmed = TrimSpaces(line);
	size_t spacePos = trimmed.find_first_of(" \t\r\n=");
	std::string cmd = (spacePos == std::string::npos) ? trimmed : trimmed.substr(0, spacePos);
	return IsValidCommand(cmd) ? std::optional<std::string>(cmd) : std::nullopt;
}

std::string Parser::ExtractArguments(const std::string& line, const std::string& cmdName)
{
	std::string trimmed = TrimSpaces(line);
	if (trimmed.find(cmdName) == 0)
	{
		size_t argsStart = cmdName.length();
		while (argsStart < trimmed.length() && (trimmed[argsStart] == ' ' || trimmed[argsStart] == '=' || trimmed[argsStart] == '\t'))
			++argsStart;
		return trimmed.substr(argsStart);
	}
	return "";
}

bool Parser::IsValidIdentifier(const std::string& id)
{
	if (id.empty())
		return false;
	if (std::isdigit(id[0]))
		return false;
	for (char c : id)
	{
		if (!std::isalnum(c) && c != '_')
			return false;
	}
	return true;
}

bool Parser::IsValidCommand(const std::string& cmd)
{
	static const std::vector<std::string> validCmds = { "var", "let", "fn", "print", "printvars", "printfns" };
	return std::find(validCmds.begin(), validCmds.end(), cmd) != validCmds.end();
}

char Parser::FindOperator(const std::string& expr)
{
	for (char c : { '+', '-', '*', '/' })
	{
		size_t pos = expr.find(c);
		if (pos != std::string::npos && pos > 0 && pos < expr.length() - 1)
			return c;
	}
	return '\0';
}

std::optional<std::unique_ptr<IExpression>> Parser::ParseExpression(const std::string& exprStr)
{
	std::string clean = TrimSpaces(exprStr);
	clean.erase(std::remove(clean.begin(), clean.end(), ' '), clean.end());
	clean.erase(std::remove(clean.begin(), clean.end(), '\t'), clean.end());

	if (clean.empty())
		return std::nullopt;

	char op = FindOperator(clean);
	if (op != '\0')
	{
		size_t opPos = clean.find(op);
		std::string leftStr = clean.substr(0, opPos);
		std::string rightStr = clean.substr(opPos + 1);

		if (!IsValidIdentifier(leftStr) || !IsValidIdentifier(rightStr))
			return std::nullopt;

		auto left = std::make_unique<IdentifierExpression>(leftStr);
		auto right = std::make_unique<IdentifierExpression>(rightStr);
		Operation operation = Operation::Add;
		if (op == '-')
			operation = Operation::Sub;
		else if (op == '*')
			operation = Operation::Mul;
		else if (op == '/')
			operation = Operation::Div;

		return std::make_unique<BinaryExpression>(std::move(left), std::move(right), operation);
	}

	if (std::isdigit(clean[0]) || clean[0] == '.')
	{
		double value = 0.0;
		auto result = std::from_chars(clean.data(), clean.data() + clean.length(), value);
		if (result.ec == std::errc())
			return std::make_unique<NumberExpression>(value);
	}

	if (IsValidIdentifier(clean))
		return std::make_unique<IdentifierExpression>(clean);

	return std::nullopt;
}
