#ifndef STREAMIO_H
#define STREAMIO_H

#include <istream>
#include <ostream>
#include <string>

namespace StreamIO
{
void Process(std::istream& input, std::ostream& output, std::string&& searchStr, std::string&& replaceStr);
};

#endif // STREAMIO_H
