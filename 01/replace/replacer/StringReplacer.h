#ifndef STRINGREPLACER_H
#define STRINGREPLACER_H

#include <string>

class StringReplacer
{
public:
	StringReplacer(std::string&& searchStr, std::string&& replaceStr)
		: m_searchStr(std::move(searchStr))
		, m_replaceStr(std::move(replaceStr))
	{
	}

	std::string Replace(const std::string& targetLine) const;

private:
	std::string m_searchStr;
	std::string m_replaceStr;
};

#endif // STRINGREPLACER_H
