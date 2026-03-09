#include "Dictionary.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

constexpr std::string_view k_dictionaryEndMarker = "===";
constexpr char k_dictionaryDelimiter = ':';
constexpr char k_translationDelimiter = ',';

Dictionary LoadDictionary(std::ifstream& in)
{
	Dictionary dict;

	if (!in.is_open())
		return dict;

	std::string line;
	while (std::getline(in, line))
	{
		if (line.empty())
			continue;
		if (line == k_dictionaryEndMarker)
			break;

		size_t pos = line.find(k_dictionaryDelimiter);
		if (pos == std::string::npos)
			throw std::runtime_error(std::format("no delimiter \'{}\' found in line \"{}\"", k_dictionaryDelimiter, line));

		std::string word = line.substr(0, pos);
		std::string rest = line.substr(pos + 1);

		std::stringstream ss(rest);
		std::string translation;

		while (std::getline(ss, translation, k_translationDelimiter))
			if (!translation.empty())
				dict[word].insert(translation);
	}

	return dict;
}

std::set<std::string> FindTranslation(const Dictionary& dict, const std::string& word)
{
	if (const auto it = dict.find(word); it != dict.end())
		return it->second;
	return {};
}

void AddTranslation(Dictionary& dict, const std::string& word, const std::string& translation)
{
	dict[word].insert(translation);
}

void SaveDictionary(std::ostream& out, const Dictionary& dict)
{
	for (const auto& [word, translations] : dict)
	{
		out << word << k_dictionaryDelimiter;

		bool first = true;
		for (const auto& tr : translations)
		{
			if (!first)
				out << k_translationDelimiter;

			out << tr;
			first = false;
		}

		out << "\n";
	}
}