#include "OutputFormatter.h"
#include <cmath>
#include <iomanip>
#include <sstream>

std::string OutputFormatter::FormatValue(std::optional<double> value)
{
	if (!value.has_value() || std::isnan(value.value()))
		return "nan";
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2) << value.value();
	return oss.str();
}
