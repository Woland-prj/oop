#ifndef HTTP_URL_H
#define HTTP_URL_H

#include <string>

enum class Protocol
{
	HTTP,
	HTTPS
};

class HttpUrl
{
public:
	explicit HttpUrl(std::string const& url);

	HttpUrl(
		std::string const& domain,
		std::string const& document,
		Protocol protocol = Protocol::HTTP);

	HttpUrl(
		std::string const& domain,
		std::string const& document,
		Protocol protocol,
		unsigned short port);

	std::string GetURL() const;
	std::string GetDomain() const;
	std::string GetDocument() const;
	Protocol GetProtocol() const;
	unsigned short GetPort() const;

private:
	std::string m_domain;
	std::string m_document;
	Protocol m_protocol;
	unsigned short m_port;

	static bool IsValidDomain(const std::string& domain);
	static bool IsValidPort(unsigned short port);
	static unsigned short GetDefaultPort(Protocol protocol);

	static Protocol ParseProtocol(const std::string& protocolStr);
	static std::string ToLower(std::string str);
	static void ValidateAndSetDocument(const std::string& document, std::string& outDocument);
};

#endif // HTTP_URL_H
