#include "Calculator.h"
#include "dispatcher/CommandHandlers.h"
#include "parser/Parser.h"
#include "types/CommandResult.h"
#include <iostream>

Calculator::Calculator(std::istream& in, std::ostream& out)
	: m_in(in)
	, m_out(out)
{
	SetupHandlers();
}

void Calculator::SetupHandlers()
{
	using namespace CommandHandlers;
	m_dispatcher.Register("var", HandleVar);
	m_dispatcher.Register("let", HandleLet);
	m_dispatcher.Register("fn", HandleFn);
	m_dispatcher.Register("print", HandlePrint);
	m_dispatcher.Register("printvars", HandlePrintVars);
	m_dispatcher.Register("printfns", HandlePrintFns);
}

void Calculator::Run()
{
	std::string line;
	while (std::getline(m_in, line))
	{
		if (line.empty())
			continue;

		auto cmdOpt = Parser::ParseCommandName(line);
		if (!cmdOpt)
		{
			std::cout << CommandResultToString(CommandResult::UnknownCommand) << "\n";
			continue;
		}

		std::string cmd = std::move(cmdOpt.value());
		std::string argsStr = Parser::ExtractArguments(line, cmd);
		std::istringstream argsStream(argsStr);

		CommandResult result = m_dispatcher.Dispatch(cmd, argsStream, m_context);
		if (result != CommandResult::Success)
			m_out << CommandResultToString(result) << '\n';
	}
}
