#include "HttpUrl.h"
#include "UrlParsingError.h"
#include <exception>
#include <execinfo.h>
#include <iostream>
#include <string>

void PrintUrlInfo(const HttpUrl& httpUrl)
{
	std::cout << "URL: " << httpUrl.GetURL() << '\n'
			  << "Protocol: " << (httpUrl.GetProtocol() == Protocol::HTTP ? "http" : "https") << '\n'
			  << "Domain: " << httpUrl.GetDomain() << '\n'
			  << "Port: " << httpUrl.GetPort() << '\n'
			  << "Document: " << httpUrl.GetDocument() << '\n';
}

void PrintStacktrace()
{
	const int max_frames = 64;
	void* frames[max_frames];

	int frame_count = backtrace(frames, max_frames);

	char** symbols = backtrace_symbols(frames, frame_count);

	if (symbols == nullptr)
	{
		std::cerr << "backtrace_symbols failed\n";
		return;
	}

	for (int i = 0; i < frame_count; ++i)
	{
		std::cout << symbols[i] << std::endl;
	}

	free(symbols);
}

int main()
{
	std::string line;

	while (std::getline(std::cin, line))
	{
		if (line.empty())
			continue;

		try
		{
			HttpUrl httpUrl(line);
			PrintUrlInfo(httpUrl);
		}
		catch (const UrlParsingError& e)
		{
			std::cerr << "Error parsing URL '" << line << "': " << e.what() << '\n';
			PrintStacktrace();
		}
		catch (const std::invalid_argument& e)
		{
			std::cerr << "Invalid argument for URL '" << line << "': " << e.what() << '\n';
			PrintStacktrace();
		}
		catch (const std::exception& e)
		{
			std::cerr << "Unexpected error processing URL '" << line << "': " << e.what() << '\n';
			PrintStacktrace();
		}
	}

	return 0;
}
