#include "StringReplacer.h"

std::string StringReplacer::Replace(const std::string& targetLine) const
{
	if (m_searchStr.empty() || m_searchStr == m_replaceStr)
		return targetLine;

	std::string res{};
	size_t searchPos = 0;

	while (true)
	{
		size_t foundPos = targetLine.find(m_searchStr, searchPos);
		if (foundPos == std::string::npos)
		{
			res.append(targetLine, searchPos, std::string::npos);
			break;
		}
		res.append(targetLine, searchPos, foundPos - searchPos);
		res += m_replaceStr;
		searchPos = foundPos + m_searchStr.length();
	}

	return res;
}
