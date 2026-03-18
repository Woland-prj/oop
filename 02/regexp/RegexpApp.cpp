#include "RegexpApp.hpp"
#include "URLParser.hpp"
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>

void RunRegexpApp(std::istream& in, std::ostream& out)
{
	std::string url;
	while (getline(in, url))
	{
		Protocol protocol;
		int port;
		std::string host, document;

		if (!ParseURL(url, protocol, port, host, document))
			throw std::invalid_argument("inval url address");

		out << url << std::endl;
		out << "HOST: " << host << std::endl;
		out << "PORT: " << port << std::endl;
		out << "DOC: " << document << std::endl;
	}
}
