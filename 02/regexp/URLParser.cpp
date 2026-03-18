#include "URLParser.hpp"
#include <algorithm>
#include <cstddef>
#include <regex>
#include <string>
#include <string_view>

constexpr std::string_view k_httpName = "http";
constexpr std::string_view k_httpsName = "https";
constexpr std::string_view k_ftpName = "ftp";

constexpr int k_stdHttpPort = 80;
constexpr int k_stdHttpsPort = 443;
constexpr int k_stdFtpPort = 21;

constexpr int k_minExistingPort = 1;
constexpr int k_maxExistingPort = 65535;

constexpr size_t k_matchIndexProto = 1;
constexpr size_t k_matchIndexHost = 2;
constexpr size_t k_matchIndexPort = 3;
constexpr size_t k_matchIndexDoc = 4;

bool ParseURL(std::string const& url, Protocol& protocol, int& port, std::string& host, std::string& document)
{
	static const std::regex urlRegex(
		R"(^(http|https|ftp)://([^/:]+)(?::(\d+))?(?:/(.*))?$)",
		std::regex::icase);

	std::smatch match;

	if (!std::regex_match(url, match, urlRegex))
		return false;

	std::string proto = match[k_matchIndexProto].str();
	std::transform(proto.begin(), proto.end(), proto.begin(), ::tolower);

	if (proto == k_httpName)
	{
		protocol = Protocol::HTTP;
		port = k_stdHttpPort;
	}
	else if (proto == k_httpsName)
	{
		protocol = Protocol::HTTPS;
		port = k_stdHttpsPort;
	}
	else if (proto == k_ftpName)
	{
		protocol = Protocol::FTP;
		port = k_stdFtpPort;
	}

	host = match[k_matchIndexHost].str();

	if (match[k_matchIndexPort].matched)
	{
		int p = std::stoi(match[k_matchIndexPort].str());
		if (p < k_minExistingPort || p > k_maxExistingPort)
			return false;
		port = p;
	}

	if (match[k_matchIndexDoc].matched)
		document = match[4].str();
	else
		document.clear();

	return true;
}
