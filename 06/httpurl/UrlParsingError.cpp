#include "UrlParsingError.h"

UrlParsingError::UrlParsingError(const std::string& message)
	: std::invalid_argument(message)
{
}
