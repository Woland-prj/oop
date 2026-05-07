#define CATCH_CONFIG_MAIN
#include "UrlParsingError.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("UrlParsingError inherits from std::invalid_argument", "[UrlParsingError]")
{
	REQUIRE_THROWS_AS(throw UrlParsingError("test error"), std::invalid_argument);
}

TEST_CASE("UrlParsingError stores message correctly", "[UrlParsingError]")
{
	try
	{
		throw UrlParsingError("Invalid URL format");
	}
	catch (const std::invalid_argument& e)
	{
		REQUIRE(std::string(e.what()) == "Invalid URL format");
	}
}
