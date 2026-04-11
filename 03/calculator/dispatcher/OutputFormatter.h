#ifndef OUTPUT_FORMATTER_H
#define OUTPUT_FORMATTER_H

#include <optional>
#include <string>

class OutputFormatter
{
public:
	static std::string FormatValue(std::optional<double> value);
};

#endif // OUTPUT_FORMATTER_H
