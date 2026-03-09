#include "HtmlDecoding.hpp"

#include <array>
#include <string>
#include <string_view>

using HtmlTokenPair = std::pair<std::string_view, char>;
constexpr size_t k_tokenCount = 5;
constexpr char k_delimetr = '&';

constexpr std::array<HtmlTokenPair, k_tokenCount> k_htmlTokens = {{
	{ "&quot;", '"'  },
	{ "&apos;", '\'' },
	{ "&lt;",   '<'  },
	{ "&gt;",   '>'  },
	{ "&amp;",  '&'  }
}};

bool TryDecodeHtmlToken(
	const std::string& html,
	std::string::size_type pos,
	char& decodedChar,
	std::string::size_type& tokenLength)
{
	for (const auto& [token, symbol] : k_htmlTokens)
	{
		if (html.compare(pos, token.length(), token) == 0)
		{
			decodedChar = symbol;
			tokenLength = token.length();
			return true;
		}
	}
	return false;
}

std::string HtmlDecode(const std::string& html)
{
	std::string result;
	result.reserve(html.size());

	std::string::size_type pos = 0;

	while (pos < html.size())
	{
		const auto delimetrPos = html.find(k_delimetr, pos);

		if (delimetrPos == std::string::npos)
		{
			result.append(html, pos, html.size() - pos);
			break;
		}

		result.append(html, pos, delimetrPos - pos);

		char decodedChar = 0;
		std::string::size_type tokenLength = 0;

		if (TryDecodeHtmlToken(html, delimetrPos, decodedChar, tokenLength))
		{
			result.push_back(decodedChar);
			pos = delimetrPos + tokenLength;
		}
		else
		{
			result.push_back(k_delimetr);
			pos = delimetrPos + 1;
		}
	}

	return result;
}