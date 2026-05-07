#include "HttpUrl.h"
#include "UrlParsingError.h"
#include "Utf8Validation.h"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <format>
#include <regex>
#include <sstream>

constexpr std::string_view HTTP_NAME = "http";
constexpr std::string_view HTTPS_NAME = "https";

constexpr unsigned short STD_HTTP_PORT = 80;
constexpr unsigned short STD_HTTPS_PORT = 443;

constexpr unsigned short MIN_PORT = 1;
constexpr unsigned short MAX_PORT = 65535;

constexpr size_t MATCH_INDEX_PROTO = 1;
constexpr size_t MATCH_INDEX_HOST = 2;
constexpr size_t MATCH_INDEX_PORT = 3;
constexpr size_t MATCH_INDEX_DOC = 4;

constexpr size_t DNS_DOMAIN_MAX_LENGTH = 253;

static const std::regex URL_REGEX(
	std::format(R"(^({}|{})://([^/:?#]+)(?::(\d+))?(?:/(.*))?$)", HTTP_NAME, HTTPS_NAME),
	std::regex::icase);

std::string HttpUrl::ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return str;
}

bool HttpUrl::IsValidPort(unsigned short port)
{
	return port >= MIN_PORT && port <= MAX_PORT;
}

unsigned short HttpUrl::GetDefaultPort(Protocol protocol)
{
	return (protocol == Protocol::HTTP) ? STD_HTTP_PORT : STD_HTTPS_PORT;
}

Protocol HttpUrl::ParseProtocol(const std::string& protocolStr)
{
	std::string lower = ToLower(protocolStr);
	if (lower == HTTP_NAME)
		return Protocol::HTTP;
	if (lower == HTTPS_NAME)
		return Protocol::HTTPS;
	throw UrlParsingError("Unknown protocol: " + protocolStr);
}

bool HttpUrl::IsValidDomain(const std::string& domain)
{
	if (domain.empty() || domain.size() > DNS_DOMAIN_MAX_LENGTH)
		return false;

	if (!utf8::IsValid(domain))
		return false;

	for (size_t i = 0; i < domain.size(); ++i)
	{
		unsigned char c = static_cast<unsigned char>(domain[i]);

		if (c < 0x80)
		{
			bool isAlphaNum = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
			if (!isAlphaNum && c != '.' && c != '-')
				return false;
		}
	}

	if (domain.front() == '-' || domain.front() == '.' || domain.back() == '-' || domain.back() == '.')
		return false;

	return true;
}

void HttpUrl::ValidateAndSetDocument(const std::string& document, std::string& outDocument)
{
	if (document.empty() || document[0] != '/')
		outDocument = "/" + document;
	else
		outDocument = document;
}

HttpUrl::HttpUrl(std::string const& url)
{
	std::smatch match;
	if (!std::regex_match(url, match, URL_REGEX))
		throw UrlParsingError("Invalid URL format: " + url);

	std::string protoStr = match[MATCH_INDEX_PROTO].str();
	m_protocol = ParseProtocol(protoStr);

	m_domain = match[MATCH_INDEX_HOST].str();
	if (!IsValidDomain(m_domain))
		throw UrlParsingError("Invalid domain: " + m_domain);

	if (match[MATCH_INDEX_PORT].matched)
	{
		try
		{
			unsigned long port = std::stoul(match[MATCH_INDEX_PORT].str());

			if (port < MIN_PORT || port > MAX_PORT)
				throw UrlParsingError("Port out of range: " + match[MATCH_INDEX_PORT].str());

			m_port = static_cast<unsigned short>(port);
		}
		catch (const std::exception&)
		{
			throw UrlParsingError("Invalid port format: " + match[MATCH_INDEX_PORT].str());
		}
	}
	else
	{
		m_port = GetDefaultPort(m_protocol);
	}

	ValidateAndSetDocument(
		match[MATCH_INDEX_DOC].matched ? match[MATCH_INDEX_DOC].str() : "",
		m_document);
}

HttpUrl::HttpUrl(
	std::string const& domain,
	std::string const& document,
	Protocol protocol)
	: HttpUrl(domain, document, protocol, GetDefaultPort(protocol))
{
}

HttpUrl::HttpUrl(
	std::string const& domain,
	std::string const& document,
	Protocol protocol,
	unsigned short port)
{
	if (!IsValidDomain(domain))
		throw std::invalid_argument("Invalid domain: " + domain);

	if (!IsValidPort(port))
		throw std::invalid_argument("Invalid port: " + std::to_string(port));

	m_domain = domain;
	m_protocol = protocol;
	m_port = port;

	ValidateAndSetDocument(document, m_document);
}

std::string HttpUrl::GetDomain() const
{
	return m_domain;
}

std::string HttpUrl::GetDocument() const
{
	return m_document;
}

Protocol HttpUrl::GetProtocol() const
{
	return m_protocol;
}

unsigned short HttpUrl::GetPort() const
{
	return m_port;
}

std::string HttpUrl::GetURL() const
{
	std::ostringstream oss;

	oss << (m_protocol == Protocol::HTTP ? HTTP_NAME : HTTPS_NAME) << "://";
	oss << m_domain;

	if (m_port != GetDefaultPort(m_protocol))
		oss << ":" << m_port;

	oss << m_document;

	return oss.str();
}
