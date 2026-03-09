#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "HtmlDecoding.hpp"

#include <string>

//
// ----------- Basic tests -----------
//

TEST_CASE("HtmlDecode handles empty string", "[HtmlDecode]")
{
	REQUIRE(HtmlDecode("") == "");
}

TEST_CASE("HtmlDecode returns string without entities unchanged", "[HtmlDecode]")
{
	REQUIRE(HtmlDecode("Hello world") == "Hello world");
}

//
// ----------- Single entity tests -----------
//

TEST_CASE("HtmlDecode decodes &lt;", "[HtmlDecode]")
{
	REQUIRE(HtmlDecode("&lt;") == "<");
}

TEST_CASE("HtmlDecode decodes &gt;", "[HtmlDecode]")
{
	REQUIRE(HtmlDecode("&gt;") == ">");
}

TEST_CASE("HtmlDecode decodes &amp;", "[HtmlDecode]")
{
	REQUIRE(HtmlDecode("&amp;") == "&");
}

TEST_CASE("HtmlDecode decodes &quot;", "[HtmlDecode]")
{
	REQUIRE(HtmlDecode("&quot;") == "\"");
}

TEST_CASE("HtmlDecode decodes &apos;", "[HtmlDecode]")
{
	REQUIRE(HtmlDecode("&apos;") == "'");
}

//
// ----------- Mixed content tests -----------
//

TEST_CASE("HtmlDecode decodes entities inside text", "[HtmlDecode]")
{
	REQUIRE(
		HtmlDecode("Cat &lt;says&gt; &quot;Meow&quot;")
		==
		"Cat <says> \"Meow\""
	);
}

TEST_CASE("HtmlDecode decodes multiple entities in a row", "[HtmlDecode]")
{
	REQUIRE(HtmlDecode("&lt;&gt;&amp;") == "<>&");
}

TEST_CASE("HtmlDecode decodes complex example from task", "[HtmlDecode]")
{
	REQUIRE(
		HtmlDecode("Cat &lt;says&gt; &quot;Meow&quot;. M&amp;M&apos;s")
		==
		"Cat <says> \"Meow\". M&M's"
	);
}

//
// ----------- Edge cases -----------
//

TEST_CASE("HtmlDecode leaves unknown entity unchanged", "[HtmlDecode]")
{
	REQUIRE(HtmlDecode("&unknown;") == "&unknown;");
}

TEST_CASE("HtmlDecode leaves incomplete entity unchanged", "[HtmlDecode]")
{
	REQUIRE(HtmlDecode("&lt") == "&lt");
}

TEST_CASE("HtmlDecode leaves single ampersand unchanged", "[HtmlDecode]")
{
	REQUIRE(HtmlDecode("&") == "&");
}

TEST_CASE("HtmlDecode handles text ending with ampersand", "[HtmlDecode]")
{
	REQUIRE(HtmlDecode("Hello &") == "Hello &");
}

//
// ----------- Nested / tricky cases -----------
//

TEST_CASE("HtmlDecode decodes &amp;lt; into &lt; (single pass)", "[HtmlDecode]")
{
	// Важно: декодирование происходит один раз
	REQUIRE(HtmlDecode("&amp;lt;") == "&lt;");
}

TEST_CASE("HtmlDecode handles repeated entities correctly", "[HtmlDecode]")
{
	REQUIRE(HtmlDecode("&amp;&amp;&amp;") == "&&&");
}

//
// ----------- Idempotency test -----------
//

TEST_CASE("HtmlDecode is idempotent on decoded string", "[HtmlDecode]")
{
	std::string text = "Hello <world> & \"quotes\"";
	REQUIRE(HtmlDecode(text) == text);
}