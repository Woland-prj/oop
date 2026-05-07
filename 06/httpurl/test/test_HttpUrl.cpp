#include "HttpUrl.h"
#include "UrlParsingError.h"
#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("HttpUrl: Parses valid HTTP URL without port", "[HttpUrl][parse][valid]")
{
	HttpUrl url("http://example.com/index.html");

	REQUIRE(url.GetProtocol() == Protocol::HTTP);
	REQUIRE(url.GetDomain() == "example.com");
	REQUIRE(url.GetDocument() == "/index.html");
	REQUIRE(url.GetPort() == 80);
}

TEST_CASE("HttpUrl: Parses valid HTTPS URL without port", "[HttpUrl][parse][valid]")
{
	HttpUrl url("https://secure.example.com/path/to/file");

	REQUIRE(url.GetProtocol() == Protocol::HTTPS);
	REQUIRE(url.GetDomain() == "secure.example.com");
	REQUIRE(url.GetDocument() == "/path/to/file");
	REQUIRE(url.GetPort() == 443);
}

TEST_CASE("HttpUrl: Parses URL with explicit default port", "[HttpUrl][parse][valid]")
{
	HttpUrl url1("http://example.com:80/page");
	REQUIRE(url1.GetPort() == 80);
	REQUIRE(url1.GetURL() == "http://example.com/page");

	HttpUrl url2("https://example.com:443/page");
	REQUIRE(url2.GetPort() == 443);
	REQUIRE(url2.GetURL() == "https://example.com/page");
}

TEST_CASE("HttpUrl: Parses URL with custom port", "[HttpUrl][parse][valid]")
{
	HttpUrl url("http://example.com:8080/api/v1");

	REQUIRE(url.GetProtocol() == Protocol::HTTP);
	REQUIRE(url.GetDomain() == "example.com");
	REQUIRE(url.GetDocument() == "/api/v1");
	REQUIRE(url.GetPort() == 8080);
	REQUIRE(url.GetURL() == "http://example.com:8080/api/v1");
}

TEST_CASE("HttpUrl: Protocol is case-insensitive", "[HttpUrl][parse][protocol]")
{
	HttpUrl url1("HTTP://example.com/");
	HttpUrl url2("http://example.com/");
	HttpUrl url3("HtTp://example.com/");
	HttpUrl url4("hTtP://example.com/");

	REQUIRE(url1.GetProtocol() == Protocol::HTTP);
	REQUIRE(url2.GetProtocol() == Protocol::HTTP);
	REQUIRE(url3.GetProtocol() == Protocol::HTTP);
	REQUIRE(url4.GetProtocol() == Protocol::HTTP);

	HttpUrl url5("HTTPS://example.com/");
	HttpUrl url6("https://example.com/");
	HttpUrl url7("HtTpS://example.com/");

	REQUIRE(url5.GetProtocol() == Protocol::HTTPS);
	REQUIRE(url6.GetProtocol() == Protocol::HTTPS);
	REQUIRE(url7.GetProtocol() == Protocol::HTTPS);
}

TEST_CASE("HttpUrl: Handles missing document (adds leading slash)", "[HttpUrl][parse][document]")
{
	HttpUrl url1("http://example.com");
	REQUIRE(url1.GetDocument() == "/");

	HttpUrl url2("https://example.com");
	REQUIRE(url2.GetDocument() == "/");
}

TEST_CASE("HttpUrl: Handles document with leading slash", "[HttpUrl][parse][document]")
{
	HttpUrl url("http://example.com/index.html");
	REQUIRE(url.GetDocument() == "/index.html");

	HttpUrl url2("https://example.com/images/photo.jpg");
	REQUIRE(url2.GetDocument() == "/images/photo.jpg");
}

TEST_CASE("HttpUrl: Handles complex domain names", "[HttpUrl][parse][domain]")
{
	HttpUrl url1("http://sub.example.com/path");
	REQUIRE(url1.GetDomain() == "sub.example.com");

	HttpUrl url2("https://my-site.example.co.uk/");
	REQUIRE(url2.GetDomain() == "my-site.example.co.uk");

	HttpUrl url3("http://site123.example.com/");
	REQUIRE(url3.GetDomain() == "site123.example.com");
}

TEST_CASE("HttpUrl: Throws on invalid protocol", "[HttpUrl][parse][invalid]")
{
	REQUIRE_THROWS_AS(HttpUrl("ftp://example.com/"), UrlParsingError);
	REQUIRE_THROWS_AS(HttpUrl("file://example.com/"), UrlParsingError);
	REQUIRE_THROWS_AS(HttpUrl("://example.com/"), UrlParsingError);
	REQUIRE_THROWS_AS(HttpUrl("httpx://example.com/"), UrlParsingError);
	REQUIRE_THROWS_AS(HttpUrl("httpsx://example.com/"), UrlParsingError);
}

TEST_CASE("HttpUrl: Throws on missing authority", "[HttpUrl][parse][invalid]")
{
	REQUIRE_THROWS_AS(HttpUrl("http://"), UrlParsingError);
	REQUIRE_THROWS_AS(HttpUrl("https://"), UrlParsingError);
	REQUIRE_THROWS_AS(HttpUrl("http:///path"), UrlParsingError);
}

TEST_CASE("HttpUrl: Throws on invalid domain", "[HttpUrl][parse][invalid]")
{
	// Пустой домен
	REQUIRE_THROWS_AS(HttpUrl("http:///path"), UrlParsingError);

	// Домен с недопустимыми символами
	REQUIRE_THROWS_AS(HttpUrl("http://exam ple.com/"), UrlParsingError);
	REQUIRE_THROWS_AS(HttpUrl("http://example@com/"), UrlParsingError);

	// Домен, начинающийся/заканчивающийся на дефис или точку
	REQUIRE_THROWS_AS(HttpUrl("http://-example.com/"), UrlParsingError);
	REQUIRE_THROWS_AS(HttpUrl("http://.example.com/"), UrlParsingError);
	REQUIRE_THROWS_AS(HttpUrl("http://example.com./"), UrlParsingError);

	// Слишком длинный домен (> 253 символа)
	REQUIRE_THROWS_AS(HttpUrl("http://" + std::string(254, 'a') + "/"), UrlParsingError);
}

TEST_CASE("HttpUrl: Throws on invalid port", "[HttpUrl][parse][invalid]")
{
	REQUIRE_THROWS_AS(HttpUrl("http://example.com:0/"), UrlParsingError);

	REQUIRE_THROWS_AS(HttpUrl("http://example.com:65536/"), UrlParsingError);
	REQUIRE_THROWS_AS(HttpUrl("http://example.com:99999/"), UrlParsingError);

	REQUIRE_THROWS_AS(HttpUrl("http://example.com:abc/"), UrlParsingError);
	REQUIRE_THROWS_AS(HttpUrl("http://example.com:80abc/"), UrlParsingError);
	REQUIRE_THROWS_AS(HttpUrl("http://example.com:/"), UrlParsingError);

	REQUIRE_THROWS_AS(HttpUrl("http://example.com:-1/"), UrlParsingError);
}

TEST_CASE("HttpUrl: Throws on malformed URL structure", "[HttpUrl][parse][invalid]")
{
	REQUIRE_THROWS_AS(HttpUrl("http:example.com/"), UrlParsingError);
	REQUIRE_THROWS_AS(HttpUrl("example.com/"), UrlParsingError);

	REQUIRE_THROWS_AS(HttpUrl(" http://example.com/"), UrlParsingError);

	REQUIRE_THROWS_AS(HttpUrl("example.com/http://"), UrlParsingError);
}

TEST_CASE("HttpUrl: Parameterized constructor with default port", "[HttpUrl][ctor][valid]")
{
	HttpUrl url("example.com", "page.html", Protocol::HTTP);

	REQUIRE(url.GetDomain() == "example.com");
	REQUIRE(url.GetDocument() == "/page.html"); // добавлен ведущий /
	REQUIRE(url.GetProtocol() == Protocol::HTTP);
	REQUIRE(url.GetPort() == 80);
	REQUIRE(url.GetURL() == "http://example.com/page.html");
}

TEST_CASE("HttpUrl: Parameterized constructor with custom port", "[HttpUrl][ctor][valid]")
{
	HttpUrl url("example.com", "/api/data", Protocol::HTTPS, 8443);

	REQUIRE(url.GetDomain() == "example.com");
	REQUIRE(url.GetDocument() == "/api/data");
	REQUIRE(url.GetProtocol() == Protocol::HTTPS);
	REQUIRE(url.GetPort() == 8443);
	REQUIRE(url.GetURL() == "https://example.com:8443/api/data");
}

TEST_CASE("HttpUrl: Parameterized constructor adds leading slash to document", "[HttpUrl][ctor][document]")
{
	HttpUrl url1("example.com", "page.html", Protocol::HTTP);
	REQUIRE(url1.GetDocument() == "/page.html");

	HttpUrl url2("example.com", "path/file.txt", Protocol::HTTPS);
	REQUIRE(url2.GetDocument() == "/path/file.txt");

	HttpUrl url3("example.com", "", Protocol::HTTP);
	REQUIRE(url3.GetDocument() == "/");

	HttpUrl url4("example.com", "/already/slash", Protocol::HTTP);
	REQUIRE(url4.GetDocument() == "/already/slash");
}

TEST_CASE("HttpUrl: Parameterized constructor with boundary ports", "[HttpUrl][ctor][port]")
{
	HttpUrl url1("example.com", "/", Protocol::HTTP, 1);
	REQUIRE(url1.GetPort() == 1);

	HttpUrl url2("example.com", "/", Protocol::HTTPS, 65535);
	REQUIRE(url2.GetPort() == 65535);

	HttpUrl url3("example.com", "/", Protocol::HTTP, 80);
	REQUIRE(url3.GetPort() == 80);
	REQUIRE(url3.GetURL() == "http://example.com/");

	HttpUrl url4("example.com", "/", Protocol::HTTPS, 443);
	REQUIRE(url4.GetPort() == 443);
	REQUIRE(url4.GetURL() == "https://example.com/");
}

TEST_CASE("HttpUrl: Parameterized constructor throws on invalid domain", "[HttpUrl][ctor][invalid]")
{
	REQUIRE_THROWS_AS(HttpUrl("", "/", Protocol::HTTP), std::invalid_argument);

	REQUIRE_THROWS_AS(HttpUrl("bad domain", "/", Protocol::HTTP), std::invalid_argument);
	REQUIRE_THROWS_AS(HttpUrl("exam@ple.com", "/", Protocol::HTTP), std::invalid_argument);

	REQUIRE_THROWS_AS(HttpUrl("-example.com", "/", Protocol::HTTP), std::invalid_argument);
	REQUIRE_THROWS_AS(HttpUrl(".example.com", "/", Protocol::HTTP), std::invalid_argument);

	REQUIRE_THROWS_AS(HttpUrl(std::string(254, 'a'), "/", Protocol::HTTP), std::invalid_argument);
}

TEST_CASE("HttpUrl: Parameterized constructor throws on invalid port", "[HttpUrl][ctor][invalid]")
{
	REQUIRE_THROWS_AS(HttpUrl("example.com", "/", Protocol::HTTP, 0), std::invalid_argument);

	REQUIRE_THROWS_AS(HttpUrl("example.com", "/", Protocol::HTTP, static_cast<unsigned short>(65536)), std::invalid_argument);
}

TEST_CASE("HttpUrl: Getters return correct values", "[HttpUrl][getters]")
{
	HttpUrl url("https://my.site.ru:8080/docs/readme.txt");

	REQUIRE(url.GetDomain() == "my.site.ru");
	REQUIRE(url.GetDocument() == "/docs/readme.txt");
	REQUIRE(url.GetProtocol() == Protocol::HTTPS);
	REQUIRE(url.GetPort() == 8080);
}

TEST_CASE("HttpUrl: GetURL omits default HTTP port", "[HttpUrl][GetURL]")
{
	HttpUrl url1("example.com", "/", Protocol::HTTP, 80);
	REQUIRE(url1.GetURL() == "http://example.com/");

	HttpUrl url2("example.com", "/page", Protocol::HTTP, 8080);
	REQUIRE(url2.GetURL() == "http://example.com:8080/page");

	HttpUrl url3("example.com", "/", Protocol::HTTP, 443);
	REQUIRE(url3.GetURL() == "http://example.com:443/");
}

TEST_CASE("HttpUrl: GetURL omits default HTTPS port", "[HttpUrl][GetURL]")
{
	HttpUrl url1("secure.com", "/", Protocol::HTTPS, 443);
	REQUIRE(url1.GetURL() == "https://secure.com/");

	HttpUrl url2("secure.com", "/api", Protocol::HTTPS, 4443);
	REQUIRE(url2.GetURL() == "https://secure.com:4443/api");

	HttpUrl url3("secure.com", "/", Protocol::HTTPS, 80);
	REQUIRE(url3.GetURL() == "https://secure.com:80/");
}

TEST_CASE("HttpUrl: GetURL roundtrip consistency", "[HttpUrl][GetURL]")
{
	std::string original = "https://test.ru:9000/a/b";
	HttpUrl url1(original);
	std::string canonical = url1.GetURL();
	HttpUrl url2(canonical);

	REQUIRE(url2.GetDomain() == url1.GetDomain());
	REQUIRE(url2.GetDocument() == url1.GetDocument());
	REQUIRE(url2.GetProtocol() == url1.GetProtocol());
	REQUIRE(url2.GetPort() == url1.GetPort());
	REQUIRE(url2.GetURL() == canonical);
}

TEST_CASE("HttpUrl: GetURL with complex document paths", "[HttpUrl][GetURL]")
{
	HttpUrl url("http://example.com/a//b///c?query=1#fragment");
	REQUIRE(url.GetDocument() == "/a//b///c?query=1#fragment");
	REQUIRE(url.GetURL() == "http://example.com/a//b///c?query=1#fragment");
}

TEST_CASE("HttpUrl: Handles minimal valid inputs", "[HttpUrl][edge]")
{
	HttpUrl url1("http://a/b");
	REQUIRE(url1.GetDomain() == "a");
	REQUIRE(url1.GetDocument() == "/b");

	HttpUrl url2("http://a:1/");
	REQUIRE(url2.GetPort() == 1);

	HttpUrl url3("http://a:65535/");
	REQUIRE(url3.GetPort() == 65535);
}

TEST_CASE("HttpUrl: Handles Unicode in domain (UTF-8)", "[HttpUrl][edge][unicode]")
{
	HttpUrl url("http://тест.рф/path");
	REQUIRE(url.GetDomain() == "тест.рф");
	REQUIRE(url.GetDocument() == "/path");
}

TEST_CASE("HttpUrl: Handles document with query and fragment", "[HttpUrl][edge]")
{
	HttpUrl url("https://example.com/page?key=value#section");
	REQUIRE(url.GetDocument() == "/page?key=value#section");
	REQUIRE(url.GetURL() == "https://example.com/page?key=value#section");
}

TEST_CASE("HttpUrl: Domain with multiple subdomains and hyphens", "[HttpUrl][edge]")
{
	HttpUrl url("http://my-long-subdomain.example-site.co.uk/path");
	REQUIRE(url.GetDomain() == "my-long-subdomain.example-site.co.uk");
	REQUIRE(url.GetDocument() == "/path");
}

TEST_CASE("HttpUrl: Full workflow — parse, modify via getters, reconstruct", "[HttpUrl][integration]")
{
	HttpUrl original("https://api.example.com:8443/v1/users?id=123");

	REQUIRE(original.GetProtocol() == Protocol::HTTPS);
	REQUIRE(original.GetDomain() == "api.example.com");
	REQUIRE(original.GetPort() == 8443);
	REQUIRE(original.GetDocument() == "/v1/users?id=123");

	HttpUrl reconstructed(
		original.GetDomain(),
		original.GetDocument(),
		original.GetProtocol(),
		original.GetPort());

	REQUIRE(reconstructed.GetURL() == original.GetURL());
}

TEST_CASE("HttpUrl: Exception messages are informative", "[HttpUrl][integration]")
{
	try
	{
		HttpUrl("invalid");
		FAIL("Expected UrlParsingError");
	}
	catch (const UrlParsingError& e)
	{
	}

	try
	{
		HttpUrl("http://example.com:99999/");
		FAIL("Expected UrlParsingError");
	}
	catch (const UrlParsingError& e)
	{
	}

	try
	{
		HttpUrl("bad@domain", "/", Protocol::HTTP);
		FAIL("Expected std::invalid_argument");
	}
	catch (const std::invalid_argument& e)
	{
		REQUIRE(std::string(e.what()).find("Invalid domain") != std::string::npos);
	}
}
