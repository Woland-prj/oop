#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "context/Context.h"
#include "dispatcher/DispatcherMap.h"
#include <istream>
#include <ostream>

class Calculator
{
public:
	Calculator(std::istream& in, std::ostream& out);
	void Run();

private:
	Context m_context;
	DispatcherMap m_dispatcher;
	std::istream& m_in;
	std::ostream& m_out;

	void SetupHandlers();
};

#endif // CALCULATOR_H
