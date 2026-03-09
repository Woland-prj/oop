#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "Dictionary.hpp"

#include <fstream>
#include <sstream>
#include <string>

static std::ifstream CreateTestFile(const std::string& filename, const std::string& content)
{
	std::ofstream file(filename);
	file << content;
	file.close();

	return std::ifstream(filename);
}

// LoadDictionary

TEST_CASE("LoadDictionary loads empty dictionary from empty file", "[LoadDictionary]")
{
	auto input = CreateTestFile("test_dict_empty.txt", "");

	auto dict = LoadDictionary(input);

	REQUIRE(dict.empty());
}

TEST_CASE("LoadDictionary loads single word with single translation", "[LoadDictionary]")
{
	auto input = CreateTestFile("test_dict_single.txt", "cat:кот\n");

	auto dict = LoadDictionary(input);

	REQUIRE(dict.size() == 1);
	REQUIRE(dict.contains("cat"));
	REQUIRE(dict.at("cat").contains("кот"));
}

TEST_CASE("LoadDictionary loads word with multiple translations", "[LoadDictionary]")
{
	auto input = CreateTestFile("test_dict_multi_trans.txt", "cat:кот,кошка\n");

	auto dict = LoadDictionary(input);

	REQUIRE(dict.size() == 1);
	REQUIRE(dict.at("cat").size() == 2);
	REQUIRE(dict.at("cat").contains("кот"));
	REQUIRE(dict.at("cat").contains("кошка"));
}

TEST_CASE("LoadDictionary loads multiple words", "[LoadDictionary]")
{
	auto input = CreateTestFile(
		"test_dict_multi_words.txt",
		"cat:кот,кошка\n"
		"dog:собака\n"
	);

	auto dict = LoadDictionary(input);

	REQUIRE(dict.size() == 2);
	REQUIRE(dict.contains("cat"));
	REQUIRE(dict.contains("dog"));
	REQUIRE(dict.at("dog").contains("собака"));
}

TEST_CASE("LoadDictionary stops reading at end marker", "[LoadDictionary]")
{
	auto input = CreateTestFile(
		"test_dict_end_marker.txt",
		"cat:кот\n"
		"===\n"
		"dog:собака\n"
	);

	auto dict = LoadDictionary(input);

	REQUIRE(dict.size() == 1);
	REQUIRE(dict.contains("cat"));
	REQUIRE_FALSE(dict.contains("dog"));
}

// FindTranslation

TEST_CASE("FindTranslation returns translations for existing word", "[FindTranslation]")
{
	Dictionary dict;
	dict["cat"] = {"кот", "кошка"};

	auto result = FindTranslation(dict, "cat");

	REQUIRE(result.size() == 2);
	REQUIRE(result.contains("кот"));
	REQUIRE(result.contains("кошка"));
}

TEST_CASE("FindTranslation returns empty set for unknown word", "[FindTranslation]")
{
	Dictionary dict;
	dict["cat"] = {"кот"};

	auto result = FindTranslation(dict, "dog");

	REQUIRE(result.empty());
}

// AddTranslation

TEST_CASE("AddTranslation adds translation to new word", "[AddTranslation]")
{
	Dictionary dict;

	AddTranslation(dict, "cat", "кот");

	REQUIRE(dict.size() == 1);
	REQUIRE(dict["cat"].contains("кот"));
}

TEST_CASE("AddTranslation adds multiple translations", "[AddTranslation]")
{
	Dictionary dict;

	AddTranslation(dict, "cat", "кот");
	AddTranslation(dict, "cat", "кошка");

	REQUIRE(dict["cat"].size() == 2);
}

TEST_CASE("AddTranslation does not duplicate translations", "[AddTranslation]")
{
	Dictionary dict;

	AddTranslation(dict, "cat", "кот");
	AddTranslation(dict, "cat", "кот");

	REQUIRE(dict["cat"].size() == 1);
}

// SaveDictionary

TEST_CASE("SaveDictionary writes empty dictionary", "[SaveDictionary]")
{
	Dictionary dict;
	std::stringstream output;

	SaveDictionary(output, dict);

	REQUIRE(output.str().empty());
}

TEST_CASE("SaveDictionary writes single word", "[SaveDictionary]")
{
	Dictionary dict;
	dict["cat"] = {"кот"};

	std::stringstream output;
	SaveDictionary(output, dict);

	REQUIRE(output.str() == "cat:кот\n");
}

TEST_CASE("SaveDictionary writes multiple translations", "[SaveDictionary]")
{
	Dictionary dict;
	dict["cat"] = {"кот", "кошка"};

	std::stringstream output;
	SaveDictionary(output, dict);

	std::string result = output.str();

	REQUIRE(result.find("cat:") != std::string::npos);
	REQUIRE(result.find("кот") != std::string::npos);
	REQUIRE(result.find("кошка") != std::string::npos);
}

TEST_CASE("SaveDictionary writes multiple words", "[SaveDictionary]")
{
	Dictionary dict;
	dict["cat"] = {"кот"};
	dict["dog"] = {"собака"};

	std::stringstream output;
	SaveDictionary(output, dict);

	std::string result = output.str();

	REQUIRE(result.find("cat:кот") != std::string::npos);
	REQUIRE(result.find("dog:собака") != std::string::npos);
}