#include <HtmlDecoding.hpp>
#include <iostream>

void DecodeStream(std::istream& in, std::ostream& out)
{
	for (std::string htmlLine; std::getline(in, htmlLine);)
	{
		if (htmlLine.empty())
			continue;
		out << HtmlDecode(htmlLine) << std::endl;
	}
}

int main()
{
	DecodeStream(std::cin, std::cout);
	return 0;
}