#ifndef URL_PARSER_HPP
#define URL_PARSER_HPP

#include <string>

enum class Protocol
{
	HTTP,
	HTTPS,
	FTP
};

bool ParseURL(std::string const& url, Protocol& protocol, int& port, std::string& host, std::string& document);

#endif // URL_PARSER_HPP
