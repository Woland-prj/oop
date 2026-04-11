#ifndef COMMAND_HANDLERS_H
#define COMMAND_HANDLERS_H

#include "types/CommandResult.h"
#include <sstream>

class Context;

namespace CommandHandlers
{
CommandResult HandleVar(std::istringstream& args, Context& ctx);
CommandResult HandleLet(std::istringstream& args, Context& ctx);
CommandResult HandleFn(std::istringstream& args, Context& ctx);
CommandResult HandlePrint(std::istringstream& args, Context& ctx);
CommandResult HandlePrintVars(std::istringstream& args, Context& ctx);
CommandResult HandlePrintFns(std::istringstream& args, Context& ctx);
} // namespace CommandHandlers

#endif // COMMAND_HANDLERS_H
