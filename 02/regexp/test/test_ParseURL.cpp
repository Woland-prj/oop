#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <string>

using namespace std;

enum class Protocol
{
	HTTP,
	HTTPS,
	FTP
};

bool ParseURL(string const& url, Protocol& protocol, int& port, string& host, string& document);

// valid
TEST_CASE("ParseURL handles basic valid URLs", "[basic]")
{
	Protocol protocol;
	int port;
	string host, doc;

	REQUIRE(ParseURL("http://example.com", protocol, port, host, doc));
	REQUIRE(host == "example.com");
	REQUIRE(port == 80);
	REQUIRE(doc.empty());

	REQUIRE(ParseURL("https://secure.com", protocol, port, host, doc));
	REQUIRE(port == 443);

	REQUIRE(ParseURL("ftp://files.com", protocol, port, host, doc));
	REQUIRE(port == 21);
}

TEST_CASE("ParseURL handles case-insensitive protocol", "[basic]")
{
	Protocol protocol;
	int port;
	string host, doc;

	REQUIRE(ParseURL("HtTp://example.com", protocol, port, host, doc));
	REQUIRE(port == 80);

	REQUIRE(ParseURL("HTTPS://example.com", protocol, port, host, doc));
	REQUIRE(port == 443);
}

TEST_CASE("ParseURL parses port correctly", "[port]")
{
	Protocol protocol;
	int port;
	string host, doc;

	REQUIRE(ParseURL("http://example.com:8080", protocol, port, host, doc));
	REQUIRE(port == 8080);

	REQUIRE(ParseURL("http://example.com:1", protocol, port, host, doc));
	REQUIRE(port == 1);

	REQUIRE(ParseURL("http://example.com:65535", protocol, port, host, doc));
	REQUIRE(port == 65535);
}

TEST_CASE("ParseURL parses document correctly", "[document]")
{
	Protocol protocol;
	int port;
	string host, doc;

	REQUIRE(ParseURL("http://example.com/index.html", protocol, port, host, doc));
	REQUIRE(doc == "index.html");

	REQUIRE(ParseURL("http://example.com/docs/page.html", protocol, port, host, doc));
	REQUIRE(doc == "docs/page.html");

	REQUIRE(ParseURL("http://example.com/a/b/c", protocol, port, host, doc));
	REQUIRE(doc == "a/b/c");
}

TEST_CASE("ParseURL handles query and fragment in document", "[document]")
{
	Protocol protocol;
	int port;
	string host, doc;

	REQUIRE(ParseURL("http://example.com/page?x=1&y=2#top", protocol, port, host, doc));
	REQUIRE(doc == "page?x=1&y=2#top");
}

TEST_CASE("ParseURL handles full URL", "[integration]")
{
	Protocol protocol;
	int port;
	string host, doc;

	REQUIRE(ParseURL("https://example.com:1234/path/to/doc?x=1#top", protocol, port, host, doc));
	REQUIRE(host == "example.com");
	REQUIRE(port == 1234);
	REQUIRE(doc == "path/to/doc?x=1#top");
}

// error
TEST_CASE("ParseURL fails on invalid protocol", "[error]")
{
	Protocol protocol;
	int port;
	string host, doc;

	REQUIRE_FALSE(ParseURL("htp://example.com", protocol, port, host, doc));
	REQUIRE_FALSE(ParseURL("smtp://example.com", protocol, port, host, doc));
}

TEST_CASE("ParseURL fails when missing protocol separator", "[error]")
{
	Protocol protocol;
	int port;
	string host, doc;

	REQUIRE_FALSE(ParseURL("http/example.com", protocol, port, host, doc));
	REQUIRE_FALSE(ParseURL("example.com", protocol, port, host, doc));
}

TEST_CASE("ParseURL fails on empty host", "[error]")
{
	Protocol protocol;
	int port;
	string host, doc;

	REQUIRE_FALSE(ParseURL("http://", protocol, port, host, doc));
	REQUIRE_FALSE(ParseURL("http:///path", protocol, port, host, doc));
}

TEST_CASE("ParseURL fails on invalid port (non-digit)", "[error]")
{
	Protocol protocol;
	int port;
	string host, doc;

	REQUIRE_FALSE(ParseURL("http://example.com:abc", protocol, port, host, doc));
	REQUIRE_FALSE(ParseURL("http://example.com:12a3", protocol, port, host, doc));
}

TEST_CASE("ParseURL fails on invalid port range", "[error]")
{
	Protocol protocol;
	int port;
	string host, doc;

	REQUIRE_FALSE(ParseURL("http://example.com:0", protocol, port, host, doc));
	REQUIRE_FALSE(ParseURL("http://example.com:65536", protocol, port, host, doc));
}

TEST_CASE("ParseURL fails on empty port after colon", "[error]")
{
	Protocol protocol;
	int port;
	string host, doc;

	REQUIRE_FALSE(ParseURL("http://example.com:", protocol, port, host, doc));
}

TEST_CASE("ParseURL fails on colon without host", "[error]")
{
	Protocol protocol;
	int port;
	string host, doc;

	REQUIRE_FALSE(ParseURL("http://:8080", protocol, port, host, doc));
}

// -------------------- EDGE CASES --------------------

TEST_CASE("ParseURL handles minimal valid host", "[edge]")
{
	Protocol protocol;
	int port;
	string host, doc;

	REQUIRE(ParseURL("http://a", protocol, port, host, doc));
	REQUIRE(host == "a");
}

TEST_CASE("ParseURL handles long paths", "[edge]")
{
	Protocol protocol;
	int port;
	string host, doc;

	string url = "http://example.com/" + string(1000, 'a');
	REQUIRE(ParseURL(url, protocol, port, host, doc));
	REQUIRE(doc.size() == 1000);
}

TEST_CASE("ParseURL does not modify document when no slash", "[edge]")
{
	Protocol protocol;
	int port;
	string host, doc = "initial";

	REQUIRE(ParseURL("http://example.com", protocol, port, host, doc));
	REQUIRE(doc.empty());
}

// -------------------- DEFAULT PORT TESTS --------------------

TEST_CASE("ParseURL sets default ports correctly", "[default-port]")
{
	Protocol protocol;
	int port;
	string host, doc;

	ParseURL("http://example.com", protocol, port, host, doc);
	REQUIRE(port == 80);

	ParseURL("https://example.com", protocol, port, host, doc);
	REQUIRE(port == 443);

	ParseURL("ftp://example.com", protocol, port, host, doc);
	REQUIRE(port == 21);
}
