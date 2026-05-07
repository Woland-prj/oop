#ifndef URL_PARSING_ERROR_H
#define URL_PARSING_ERROR_H

#include <stdexcept>
#include <string>

class UrlParsingError : public std::invalid_argument
{
public:
	explicit UrlParsingError(const std::string& message);
};

#endif // URL_PARSING_ERROR_H
