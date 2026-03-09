#include "DictionaryApp.hpp"

#include "Dictionary.hpp"
#include "WordTransform.hpp"

#include <fstream>
#include <iostream>
#include <string>

constexpr std::string_view k_quietMarker = "...";
constexpr std::string_view k_yesString = "y";
constexpr std::string_view k_noString = "n";

void RunSave(const Dictionary& dict, const std::string& dictFile)
{
	std::ofstream out(dictFile);
	if (!out.is_open())
		throw std::runtime_error(std::format("Cannot open dictionary file for write{}", dictFile));
	SaveDictionary(out, dict);
}

void RunSaveDialog(const Dictionary& dict, const std::string& dictFile)
{
	std::cout << "В словарь были внесены изменения. Введите Y или y для сохранения перед выходом.\n";
	std::string answer;
	do
	{
		std::getline(std::cin, answer);
		ToLower(answer);
		if (answer == k_yesString)
		{
			RunSave(dict, dictFile);
			std::cout << "Изменения сохранены. До свидания.\n";
			return;
		}
		if (answer == k_noString)
		{
			std::cout << "Изменения не сохранены. До свидания.\n";
			return;
		}
		answer.clear();
	} while (answer.empty());
}

void RunAddTranslationDialog(Dictionary& dict, const std::string& word, bool& changed)
{
	std::cout << std::format("Неизвестное слово \"{}\". Введите перевод или пустую строку для отказа.\n", word);
	std::string translation;

	std::getline(std::cin, translation);
	ToLower(translation);
	if (!translation.empty())
	{
		AddTranslation(dict, word, translation);
		std::cout << std::format("Слово \"{}\" сохранено в словаре как \"{}\"\n", word, translation);
		changed = true;
	}
	else
		std::cout << std::format("Слово \"{}\" проигнорировано.\n", word);
}

void RunUI(Dictionary& dict, const std::string& dictFile)
{
	bool changed = false;
	std::string line;

	while (true)
	{
		std::cout << ">";
		std::getline(std::cin, line);
		if (line == k_quietMarker)
		{
			if (changed)
				RunSaveDialog(dict, dictFile);
			break;
		}
		const auto translations = FindTranslation(dict, line);
		if (translations.empty())
		{
			RunAddTranslationDialog(dict, line, changed);
		}
	}
}

void RunDictionaryApp(int argc, char* argv[])
{
	if (argc != 2)
		throw std::invalid_argument("Required filename argument");
	const std::string dictFileName = argv[1];
	std::ifstream dictFile(dictFileName);
	Dictionary dict{};
	if (!dictFile.is_open())
	{
		dict = LoadDictionary(dictFile);
		dictFile.close();
	}
	RunUI(dict, dictFileName);
}