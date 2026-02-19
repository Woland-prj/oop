#ifndef STREAMIO_H
#define STREAMIO_H

#include <istream>
#include <string>

namespace StreamIO
{
void Process(std::istream& input, std::string&& searchStr, std::string&& replaceStr);
};

#endif // STREAMIO_H
