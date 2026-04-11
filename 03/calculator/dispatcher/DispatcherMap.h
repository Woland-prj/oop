#ifndef DISPATCHER_MAP_H
#define DISPATCHER_MAP_H

#include "types/CommandResult.h"
#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>

class Context;

using HandlerFn = std::function<CommandResult(std::istringstream&, Context&)>;

class DispatcherMap
{
public:
	void Register(const std::string& name, HandlerFn handler);
	CommandResult Dispatch(const std::string& cmd, std::istringstream& args, Context& ctx);

private:
	std::unordered_map<std::string, HandlerFn> m_handlers;
};

#endif // DISPATCHER_MAP_H
