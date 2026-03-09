#include "Crypting.hpp"
#include <array>
#include <cstdint>
#include <functional>

constexpr size_t k_bufSize = 32 * 1024;
using ByteFunc = std::function<uint8_t(uint8_t)>;

constexpr uint8_t DoXor(uint8_t byte, uint8_t key)
{
	return byte ^ key;
}

constexpr uint8_t DoBitMix(uint8_t byte)
{
	return ((byte & (1 << 4)) << 3) | // 4 -> 7
		((byte & (1 << 3)) << 3) | // 3 -> 6
		((byte & (1 << 7)) >> 2) | // 7 -> 5
		((byte & (1 << 2)) << 2) | // 2 -> 4
		((byte & (1 << 1)) << 2) | // 1 -> 3
		((byte & (1 << 0)) << 2) | // 0 -> 2
		((byte & (1 << 6)) >> 5) | // 6 -> 1
		((byte & (1 << 5)) >> 5); // 5 -> 0
}

constexpr uint8_t DoBitRestore(uint8_t byte)
{
	return ((byte & (1 << 7)) >> 3) | // 7 -> 4
		((byte & (1 << 6)) >> 3) | // 6 -> 3
		((byte & (1 << 5)) << 2) | // 5 -> 7
		((byte & (1 << 4)) >> 2) | // 4 -> 2
		((byte & (1 << 3)) >> 2) | // 3 -> 1
		((byte & (1 << 2)) >> 2) | // 2 -> 0
		((byte & (1 << 1)) << 5) | // 1 -> 6
		((byte & (1 << 0)) << 5); // 0 -> 5
}

void CryptLoop(std::istream& in, std::ostream& out, ByteFunc func)
{
	std::array<char, k_bufSize> buf;

	while (in.read(buf.data(), k_bufSize) || in.gcount())
	{
		std::streamsize count = in.gcount();

		for (std::streamsize i = 0; i < count; ++i)
			buf[i] = func(buf[i]);

		out.write(buf.data(), count);
	}
}

void Crypt(std::istream& in, std::ostream& out, uint8_t key)
{
	CryptLoop(in, out, [key](uint8_t b) {
		b = DoXor(b, key);
		b = DoBitMix(b);
		return b;
	});
}

void Decrypt(std::istream& in, std::ostream& out, uint8_t key)
{
	CryptLoop(in, out, [key](uint8_t b) {
		b = DoBitRestore(b);
		b = DoXor(b, key);
		return b;
	});
}
