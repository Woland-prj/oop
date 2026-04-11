#ifndef COMMAND_RESULT_H
#define COMMAND_RESULT_H

#include <string>

enum class CommandResult
{
	Success,
	UnknownCommand,
	InvalidUsage,
	NameExists,
	NameNotFound
};

inline std::string CommandResultToString(CommandResult result)
{
	switch (result)
	{
	case CommandResult::Success:
		return "";
	case CommandResult::UnknownCommand:
		return "Unknown command";
	case CommandResult::InvalidUsage:
		return "Invalid usage";
	case CommandResult::NameExists:
		return "Name already exists";
	case CommandResult::NameNotFound:
		return "Name does not exist";
	}
	return "Unknown error";
}

#endif // COMMAND_RESULT_H
