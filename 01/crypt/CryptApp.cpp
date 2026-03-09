#include "CryptApp.hpp"
#include "Crypting.hpp"

#include <charconv>
#include <cstdint>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string_view>

constexpr int k_helpArgsCount = 2;
constexpr int k_cryptArgsCount = 5;

constexpr int k_cmdIndex = 1;
constexpr int k_InputIndex = 2;
constexpr int k_OutputIndex = 3;
constexpr int k_KeyIndex = 4;

constexpr std::string_view k_cryptCmd = "crypt";
constexpr std::string_view k_decryptCmd = "decrypt";
constexpr std::string_view k_helpFlag = "--help";

void PrintHelp()
{
	std::cout << "Usage:\n"
				 "  crypt crypt <input file> <output file> <key>\n"
				 "  crypt decrypt <input file> <output file> <key>\n"
				 "  crypt --help\n\n"
				 "key: integer in range [0..255]\n";
}
void RunCrypt(char* argv[]);
void RunDecrypt(char* argv[]);

uint8_t ParseByte(const std::string_view& argStr)
{
	if (argStr.empty())
		throw std::invalid_argument("Argument is empty");
	int byte{};
	auto [ptr, ec] = std::from_chars(argStr.data(), argStr.data() + argStr.size(), byte);
	if (ec == std::errc::invalid_argument || ptr != argStr.data() + argStr.size())
		throw std::invalid_argument("Argument is not number");
	else if (ec == std::errc::result_out_of_range || byte < 0 || byte > std::numeric_limits<uint8_t>::max())
		throw std::out_of_range("Argument is larger than byte");

	return static_cast<uint8_t>(byte);
}

void RunOperation(char* argv[], CryptOperation op)
{
	std::string inputPath = argv[k_InputIndex];
	std::string outputPath = argv[k_OutputIndex];

	uint8_t key = ParseByte(std::string_view(argv[k_KeyIndex]));

	std::ifstream in(inputPath, std::ios::binary);
	if (!in)
		throw std::runtime_error(std::format("Cannot open input file: {}", inputPath));

	std::ofstream out(outputPath, std::ios::binary);
	if (!out)
		throw std::runtime_error(std::format("Cannot open output file: {}", outputPath));

	op(in, out, key);
}

void RunCryptApp(int argc, char* argv[])
{
	if (argc == k_helpArgsCount)
	{
		std::string_view arg = argv[1];

		if (arg == k_helpFlag)
		{
			PrintHelp();
			return;
		}

		throw std::invalid_argument(
			std::format("Unknown argument: {}", arg));
	}

	if (argc == k_cryptArgsCount)
	{
		std::string_view cmd = argv[1];

		if (cmd == k_cryptCmd)
		{
			RunOperation(argv, Crypt);
			return;
		}

		if (cmd == k_decryptCmd)
		{
			RunOperation(argv, Decrypt);
			return;
		}

		throw std::invalid_argument(
			std::format("Unknown command: {}", cmd));
	}

	throw std::invalid_argument(
		std::format("Invalid args count: {}", argc - 1));
}
