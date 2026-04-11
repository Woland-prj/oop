#ifndef PARSER_H
#define PARSER_H

#include "../ast/IExpression.h"
#include <memory>
#include <optional>
#include <string>

class Parser
{
public:
	static std::optional<std::string> ParseCommandName(const std::string& line);
	static std::string ExtractArguments(const std::string& line, const std::string& cmdName);
	static std::optional<std::unique_ptr<IExpression>> ParseExpression(const std::string& exprStr);
	static bool IsValidIdentifier(const std::string& id);
	static bool IsValidCommand(const std::string& cmd);
	static std::string TrimSpaces(const std::string& str);

private:
	static char FindOperator(const std::string& expr);
};

#endif // PARSER_H
