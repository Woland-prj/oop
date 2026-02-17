#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <format>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

constexpr std::size_t REQUIRED_ARGS = 2;

constexpr uint8_t Flip(uint8_t byte)
{
	byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
	byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
	byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
	return byte;
}

uint8_t ParseByte(const std::string& argStr)
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

void PrintHelp()
{
	std::cout << "Usage:\n\n"
			  << "flipbyte <byte> - print decimal representation of <byte> with reversed bits\n"
			  << "\t<byte> - decimal number in range 0-255\n\n"
			  << "flipbyte -h - show this text\n";
}

int main(int argc, char* argv[])
{
	if (argc != REQUIRED_ARGS)
	{
		std::cerr << "Requires " << REQUIRED_ARGS
				  << " arguments. See help: flipbyte -h" << std::endl;
		return 1;
	}
	if (std::string(argv[1]) == "-h")
	{
		PrintHelp();
		return 0;
	}
	uint8_t byte;
	try
	{
		byte = ParseByte(std::string(argv[1]));
	}
	catch (std::exception const& ex)
	{
		std::cerr << "ERROR: " << ex.what()
				  << " See help: flipbyte -h" << std::endl;
		return 1;
	}
	std::cout << std::format("{:d}", Flip(byte)) << std::endl;
	return 0;
}
