#include "DispatcherMap.h"
#include <unordered_map>

void DispatcherMap::Register(const std::string& name, HandlerFn handler)
{
	m_handlers[name] = std::move(handler);
}

CommandResult DispatcherMap::Dispatch(const std::string& cmd, std::istringstream& args, Context& ctx)
{
	auto it = m_handlers.find(cmd);
	if (it == m_handlers.end())
		return CommandResult::UnknownCommand;
	return it->second(args, ctx);
}
