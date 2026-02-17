#include "StreamIO.h"
#include "replacer/StringReplacer.h"
#include <ostream>
#include <string>

void StreamIO::Process(std::istream& input, std::ostream& output, std::string&& searchStr, std::string&& replaceStr)
{
	StringReplacer replacer(std::move(searchStr), std::move(replaceStr));
	std::string line;
	while (std::getline(input, line))
		output << replacer.Replace(line) << '\n';
	output.flush();
}
