#define CATCH_CONFIG_MAIN
#include "Utf8Validation.h"
#include <catch2/catch_test_macros.hpp>
#include <string>

std::string Bytes(std::initializer_list<unsigned char> bytes)
{
	std::string result;
	result.reserve(bytes.size());
	for (unsigned char b : bytes)
		result += static_cast<char>(b);
	return result;
}

TEST_CASE("GetValidSequenceLength: ASCII characters (1-byte)", "[utf8][1-byte]")
{
	// ASCII: 0xxxxxxx (0x00..0x7F)
	REQUIRE(utf8::GetValidSequenceLength("a", 0) == 1);
	REQUIRE(utf8::GetValidSequenceLength("Z", 0) == 1);
	REQUIRE(utf8::GetValidSequenceLength("0", 0) == 1);
	REQUIRE(utf8::GetValidSequenceLength(" ", 0) == 1);
	REQUIRE(utf8::GetValidSequenceLength("\x7F", 0) == 1); // DEL, ASCII

	REQUIRE(utf8::GetValidSequenceLength("a", 1) == 0);
	REQUIRE(utf8::GetValidSequenceLength("", 0) == 0);
}

TEST_CASE("GetValidSequenceLength: Valid 2-byte sequences (U+0080..U+07FF)", "[utf8][2-byte]")
{
	// Min 2 bytes: U+0080 = 0xC2 0x80
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xC2, 0x80 }), 0) == 2);

	// Cirilic: 'а' (U+0430) = 0xD0 0xB0
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xD0, 0xB0 }), 0) == 2);

	// Max 2 bytes: U+07FF = 0xDF 0xBF
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xDF, 0xBF }), 0) == 2);

	// Inside str
	REQUIRE(utf8::GetValidSequenceLength("a" + Bytes({ 0xD0, 0xB0 }) + "b", 1) == 2);
}

TEST_CASE("GetValidSequenceLength: Valid 3-byte sequences (U+0800..U+FFFF, excl. surrogates)", "[utf8][3-byte]")
{
	// Min 3 bytes: U+0800 = 0xE0 0xA0 0x80
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xE0, 0xA0, 0x80 }), 0) == 3);

	// '€' (U+20AC) = 0xE2 0x82 0xAC
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xE2, 0x82, 0xAC }), 0) == 3);

	// Max before surrogate: U+D7FF = 0xED 0x9F 0xBF
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xED, 0x9F, 0xBF }), 0) == 3);

	// After: U+E000 = 0xEE 0x80 0x80
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xEE, 0x80, 0x80 }), 0) == 3);

	// Max 3 bytes: U+FFFF = 0xEF 0xBF 0xBF
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xEF, 0xBF, 0xBF }), 0) == 3);
}

TEST_CASE("GetValidSequenceLength: Valid 4-byte sequences (U+10000..U+10FFFF)", "[utf8][4-byte]")
{
	// Мin 4 bytes: U+10000 = 0xF0 0x90 0x80 0x80
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xF0, 0x90, 0x80, 0x80 }), 0) == 4);

	// '😀' (U+1F600) = 0xF0 0x9F 0x98 0x80
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xF0, 0x9F, 0x98, 0x80 }), 0) == 4);

	// Мax 4 bytes: U+10FFFF = 0xF4 0x8F 0xBF 0xBF
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xF4, 0x8F, 0xBF, 0xBF }), 0) == 4);
}

TEST_CASE("GetValidSequenceLength: Invalid header bytes", "[utf8][invalid-header]")
{
	// 10xxxxxx
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0x80 }), 0) == 0);
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xBF }), 0) == 0);

	// 11111xxx
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xF8 }), 0) == 0);
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xFC }), 0) == 0);
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xFE }), 0) == 0);
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xFF }), 0) == 0);
}

TEST_CASE("GetValidSequenceLength: Truncated sequences", "[utf8][truncated]")
{
	// 2 bytes, without second
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xC2 }), 0) == 0);

	// 3 bytes, without last
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xE0, 0xA0 }), 0) == 0);

	// 4 bytes, without last
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xF0, 0x90, 0x80 }), 0) == 0);

	// Cutted str
	REQUIRE(utf8::GetValidSequenceLength("abc" + Bytes({ 0xD0 }), 3) == 0);
}

TEST_CASE("GetValidSequenceLength: Invalid continuation bytes", "[utf8][invalid-continuation]")
{
	// Invalid start (2 bytes)
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xC2, 0x00 }), 0) == 0);
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xC2, 0xC0 }), 0) == 0);
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xC2, 0xFF }), 0) == 0);

	// Invalid third byte
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xE0, 0xA0, 0x00 }), 0) == 0);

	// Invalid fourth byte
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xF0, 0x90, 0x80, 0x7F }), 0) == 0);
}

TEST_CASE("GetValidSequenceLength: Rejects overlong 2-byte encoding", "[utf8][overlong]")
{
	// U+0000 : 0xC0 0x80
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xC0, 0x80 }), 0) == 0);

	// U+002F ('/') : 0xC0 0xAF
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xC0, 0xAF }), 0) == 0);

	// U+007F (last ASCII): 0xC1 0xBF
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xC1, 0xBF }), 0) == 0);

	// U+0080 valid 2 bytes
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xC2, 0x80 }), 0) == 2);
}

TEST_CASE("GetValidSequenceLength: Rejects overlong 3-byte encoding", "[utf8][overlong]")
{
	// U+0000 : 0xE0 0x80 0x80
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xE0, 0x80, 0x80 }), 0) == 0);

	// U+007F : 0xE0 0x81 0xBF
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xE0, 0x81, 0xBF }), 0) == 0);

	// U+0800 valid 3 bytes
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xE0, 0xA0, 0x80 }), 0) == 3);
}

TEST_CASE("GetValidSequenceLength: Rejects overlong 4-byte encoding", "[utf8][overlong]")
{
	// U+0000 : 0xF0 0x80 0x80 0x80
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xF0, 0x80, 0x80, 0x80 }), 0) == 0);

	// U+FFFF : 0xF0 0x8F 0xBF 0xBF
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xF0, 0x8F, 0xBF, 0xBF }), 0) == 0);

	// U+10000 valid 4 bytes
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xF0, 0x90, 0x80, 0x80 }), 0) == 4);
}

TEST_CASE("GetValidSequenceLength: Rejects UTF-16 surrogates", "[utf8][surrogate]")
{
	// start
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xED, 0xA0, 0x80 }), 0) == 0);

	// middle
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xED, 0xB0, 0x80 }), 0) == 0);

	// end
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xED, 0xBF, 0xBF }), 0) == 0);

	// borders valid
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xED, 0x9F, 0xBF }), 0) == 3); // U+D7FF
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xEE, 0x80, 0x80 }), 0) == 3); // U+E000
}

TEST_CASE("GetValidSequenceLength: Rejects codepoints beyond U+10FFFF", "[utf8][out-of-range]")
{
	// Min invalid 0xF4 0x90 0x80 0x80 = U+110000
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xF4, 0x90, 0x80, 0x80 }), 0) == 0);

	// Max avaliable by format: 0xF4 0xBF 0xBF 0xBF (но > 0x10FFFF)
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xF4, 0xBF, 0xBF, 0xBF }), 0) == 0);

	// Borders valid: 0xF4 0x8F 0xBF 0xBF = U+10FFFF
	REQUIRE(utf8::GetValidSequenceLength(Bytes({ 0xF4, 0x8F, 0xBF, 0xBF }), 0) == 4);
}

TEST_CASE("ExtractCodepoint: Correctly extracts from valid sequences", "[utf8][extract]")
{
	// 2 bytes
	REQUIRE(utf8::ExtractCodepoint(Bytes({ 0xC2, 0x80 }), 0, 2) == 0x0080);
	REQUIRE(utf8::ExtractCodepoint(Bytes({ 0xD0, 0xB0 }), 0, 2) == 0x0430); // 'а'
	REQUIRE(utf8::ExtractCodepoint(Bytes({ 0xDF, 0xBF }), 0, 2) == 0x07FF);

	// 3 bytes
	REQUIRE(utf8::ExtractCodepoint(Bytes({ 0xE0, 0xA0, 0x80 }), 0, 3) == 0x0800);
	REQUIRE(utf8::ExtractCodepoint(Bytes({ 0xE2, 0x82, 0xAC }), 0, 3) == 0x20AC); // '€'
	REQUIRE(utf8::ExtractCodepoint(Bytes({ 0xEF, 0xBF, 0xBF }), 0, 3) == 0xFFFF);

	// 4 bytes
	REQUIRE(utf8::ExtractCodepoint(Bytes({ 0xF0, 0x90, 0x80, 0x80 }), 0, 4) == 0x10000);
	REQUIRE(utf8::ExtractCodepoint(Bytes({ 0xF0, 0x9F, 0x98, 0x80 }), 0, 4) == 0x1F600); // '😀'
	REQUIRE(utf8::ExtractCodepoint(Bytes({ 0xF4, 0x8F, 0xBF, 0xBF }), 0, 4) == 0x10FFFF);
}

TEST_CASE("ExtractCodepoint: Returns 0 for invalid seqLen", "[utf8][extract]")
{
	REQUIRE(utf8::ExtractCodepoint("a", 0, 1) == 0);
	REQUIRE(utf8::ExtractCodepoint("a", 0, 5) == 0);
}

TEST_CASE("IsValid: Accepts valid ASCII strings", "[utf8][IsValid][ascii]")
{
	REQUIRE(utf8::IsValid(""));
	REQUIRE(utf8::IsValid("hello"));
	REQUIRE(utf8::IsValid("example.com"));
	REQUIRE(utf8::IsValid("0123456789!@#$%^&*()"));
	REQUIRE(utf8::IsValid(std::string(1000, 'x')));
}

TEST_CASE("IsValid: Accepts valid multi-byte UTF-8 strings", "[utf8][IsValid][unicode]")
{
	// Cirilic
	REQUIRE(utf8::IsValid("пример"));
	REQUIRE(utf8::IsValid("пример.рф"));

	// Chinesse
	REQUIRE(utf8::IsValid("中文"));

	// 4 bytes
	REQUIRE(utf8::IsValid("😀🎉🚀"));

	// Mixed
	REQUIRE(utf8::IsValid("Hello, 世界! Привет, 😀"));

	// Random
	REQUIRE(utf8::IsValid("€£¥©®™"));
}

TEST_CASE("IsValid: Rejects malformed UTF-8", "[utf8][IsValid][malformed]")
{
	// single continious byte
	REQUIRE_FALSE(utf8::IsValid(Bytes({ 0x80 })));
	REQUIRE_FALSE(utf8::IsValid("abc" + Bytes({ 0xBF }) + "def"));
	REQUIRE_FALSE(utf8::IsValid(Bytes({ 0xC2, 0x00 })));
	REQUIRE_FALSE(utf8::IsValid(Bytes({ 0xE0, 0xA0, 0xFF })));

	// invalid start
	REQUIRE_FALSE(utf8::IsValid(Bytes({ 0xF8 })));
	REQUIRE_FALSE(utf8::IsValid(Bytes({ 0xFF })));

	// cut sequence
	REQUIRE_FALSE(utf8::IsValid(Bytes({ 0xC2 })));
	REQUIRE_FALSE(utf8::IsValid(Bytes({ 0xE0, 0xA0 })));
	REQUIRE_FALSE(utf8::IsValid("test" + Bytes({ 0xF0, 0x90 })));
}

TEST_CASE("IsValid: Rejects overlong encodings", "[utf8][IsValid][overlong]")
{
	REQUIRE_FALSE(utf8::IsValid(Bytes({ 0xC0, 0x80 }))); // U+0000 overlong
	REQUIRE_FALSE(utf8::IsValid(Bytes({ 0xC1, 0xBF }))); // U+007F overlong
	REQUIRE_FALSE(utf8::IsValid(Bytes({ 0xE0, 0x80, 0x80 }))); // U+0000 overlong 3-byte
	REQUIRE_FALSE(utf8::IsValid(Bytes({ 0xF0, 0x80, 0x80, 0x80 }))); // U+0000 overlong 4-byte
}

TEST_CASE("IsValid: Rejects surrogate pairs", "[utf8][IsValid][surrogate]")
{
	REQUIRE_FALSE(utf8::IsValid(Bytes({ 0xED, 0xA0, 0x80 }))); // U+D800
	REQUIRE_FALSE(utf8::IsValid(Bytes({ 0xED, 0xBF, 0xBF }))); // U+DFFF
	REQUIRE_FALSE(utf8::IsValid("valid" + Bytes({ 0xED, 0xA0, 0x80 }) + "text"));
}

TEST_CASE("IsValid: Rejects out-of-range codepoints", "[utf8][IsValid][out-of-range]")
{
	REQUIRE_FALSE(utf8::IsValid(Bytes({ 0xF4, 0x90, 0x80, 0x80 }))); // U+110000
	REQUIRE_FALSE(utf8::IsValid(Bytes({ 0xF4, 0xBF, 0xBF, 0xBF }))); // Max possible but > U+10FFFF
}

TEST_CASE("IsValid: Edge cases", "[utf8][IsValid][edge]")
{
	REQUIRE(utf8::IsValid(""));

	REQUIRE(utf8::IsValid(std::string(100, '\x20')));
	REQUIRE(utf8::IsValid(std::string(10, '\x00')));

	std::string longValid(10000, 'a');
	REQUIRE(utf8::IsValid(longValid));

	REQUIRE(utf8::IsValid(Bytes({ 0xF4, 0x8F, 0xBF, 0xBF })));
}

TEST_CASE("IsValid: Real-world domain names with Unicode", "[utf8][integration][domain]")
{
	// utf domains
	REQUIRE(utf8::IsValid("пример.рф"));
	REQUIRE(utf8::IsValid("münchen.de"));
	REQUIRE(utf8::IsValid("北京.中国"));
	REQUIRE(utf8::IsValid("café.fr"));
	REQUIRE(utf8::IsValid("straße.de"));
	REQUIRE(utf8::IsValid("test-пример.com"));
}

TEST_CASE("IsValid: Rejects common injection/malformed patterns", "[utf8][integration][security]")
{
	REQUIRE_FALSE(utf8::IsValid("/path" + Bytes({ 0xC0, 0xAF }) + "etc")); // overlong '/'

	REQUIRE(utf8::IsValid(std::string("test", 4) + '\0' + std::string("end", 3)));

	REQUIRE(utf8::IsValid(std::string(100000, '\x00')));
}
