#include "Utf8Validation.h"

namespace utf8
{

constexpr unsigned char UTF8_FIRST_NOT_ASCII_CHAR = 0x80;

constexpr unsigned char UTF8_CONTINUATION_MASK = 0xC0; // 11000000
constexpr unsigned char UTF8_CONTINUATION_PATTERN = 0x80; // 10xxxxxx

constexpr unsigned char UTF8_2BYTE_HEADER_MASK = 0xE0; // 11100000
constexpr unsigned char UTF8_2BYTE_HEADER_PATTERN = 0xC0; // 110xxxxx

constexpr unsigned char UTF8_3BYTE_HEADER_MASK = 0xF0; // 11110000
constexpr unsigned char UTF8_3BYTE_HEADER_PATTERN = 0xE0; // 1110xxxx

constexpr unsigned char UTF8_4BYTE_HEADER_MASK = 0xF8; // 11111000
constexpr unsigned char UTF8_4BYTE_HEADER_PATTERN = 0xF0; // 11110xxx

constexpr unsigned char UTF8_2BYTE_DATA_MASK = 0x1F; // 00011111
constexpr unsigned char UTF8_3BYTE_DATA_MASK = 0x0F; // 00001111
constexpr unsigned char UTF8_4BYTE_DATA_MASK = 0x07; // 00000111
constexpr unsigned char UTF8_CONTINUATION_DATA_MASK = 0x3F; // 00111111

constexpr unsigned int UNICODE_MAX_CODEPOINT = 0x10FFFF;

// overlong-encoding
constexpr unsigned int UTF8_1BYTE_MAX_CODEPOINT = 0x7F;
constexpr unsigned int UTF8_2BYTE_MIN_CODEPOINT = 0x80;
constexpr unsigned int UTF8_3BYTE_MIN_CODEPOINT = 0x800;
constexpr unsigned int UTF8_4BYTE_MIN_CODEPOINT = 0x10000;

// invalid pairs inUTF-8, RFC 3629
constexpr unsigned int UNICODE_SURROGATE_MIN = 0xD800;
constexpr unsigned int UNICODE_SURROGATE_MAX = 0xDFFF;

unsigned int ExtractCodepoint(const std::string& str, size_t pos, size_t seqLen)
{
	const unsigned char* bytes = reinterpret_cast<const unsigned char*>(str.data() + pos);

	switch (seqLen)
	{
	case 2:
		return ((bytes[0] & UTF8_2BYTE_DATA_MASK) << 6) | ((bytes[1] & UTF8_CONTINUATION_DATA_MASK));
	case 3:
		return ((bytes[0] & UTF8_3BYTE_DATA_MASK) << 12) | ((bytes[1] & UTF8_CONTINUATION_DATA_MASK) << 6) | ((bytes[2] & UTF8_CONTINUATION_DATA_MASK));
	case 4:
		return ((bytes[0] & UTF8_4BYTE_DATA_MASK) << 18) | ((bytes[1] & UTF8_CONTINUATION_DATA_MASK) << 12) | ((bytes[2] & UTF8_CONTINUATION_DATA_MASK) << 6) | ((bytes[3] & UTF8_CONTINUATION_DATA_MASK));
	default:
		return 0;
	}
}

size_t GetValidSequenceLength(const std::string& str, size_t pos)
{
	if (pos >= str.size())
		return 0;

	const unsigned char c = static_cast<unsigned char>(str[pos]);

	// ASCII - 0xxxxxxx
	if (c < UTF8_FIRST_NOT_ASCII_CHAR)
		return 1;

	size_t seqLen = 0;
	if ((c & UTF8_2BYTE_HEADER_MASK) == UTF8_2BYTE_HEADER_PATTERN)
		seqLen = 2;
	else if ((c & UTF8_3BYTE_HEADER_MASK) == UTF8_3BYTE_HEADER_PATTERN)
		seqLen = 3;
	else if ((c & UTF8_4BYTE_HEADER_MASK) == UTF8_4BYTE_HEADER_PATTERN)
		seqLen = 4;
	else
		return 0;

	if (pos + seqLen > str.size())
		return 0;

	for (size_t i = 1; i < seqLen; ++i)
	{
		if ((static_cast<unsigned char>(str[pos + i]) & UTF8_CONTINUATION_MASK)
			!= UTF8_CONTINUATION_PATTERN)
			return 0;
	}

	unsigned int codepoint = ExtractCodepoint(str, pos, seqLen);

	if (seqLen == 2 && codepoint < UTF8_2BYTE_MIN_CODEPOINT)
		return 0;
	else if (seqLen == 3)
	{
		if (codepoint < UTF8_3BYTE_MIN_CODEPOINT)
			return 0;
		if (codepoint >= UNICODE_SURROGATE_MIN && codepoint <= UNICODE_SURROGATE_MAX)
			return 0;
	}
	else if (seqLen == 4)
	{
		if (codepoint < UTF8_4BYTE_MIN_CODEPOINT)
			return 0;
		if (codepoint > UNICODE_MAX_CODEPOINT)
			return 0;
	}

	return seqLen;
}

bool IsValid(const std::string& str)
{
	for (size_t i = 0; i < str.size();)
	{
		size_t len = GetValidSequenceLength(str, i);
		if (len == 0)
			return false;
		i += len;
	}
	return true;
}

} // namespace utf8
