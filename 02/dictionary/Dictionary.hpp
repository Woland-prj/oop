#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <fstream>
#include <map>
#include <set>
#include <string>

// Переделать на unordered_map
using Dictionary = std::map<std::string, std::set<std::string>>;

Dictionary LoadDictionary(std::ifstream& in);

std::set<std::string> FindTranslation(const Dictionary& dict, const std::string& word);

void AddTranslation(Dictionary& dict, const std::string& word, const std::string& translation);

void SaveDictionary(std::ostream& out, const Dictionary& dict);

#endif // DICTIONARY_HPP
