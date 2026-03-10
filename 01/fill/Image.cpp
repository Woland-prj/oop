#include "Image.hpp"

Image LoadImage(std::istream& in)
{
	Image img;

	std::string line;
	size_t row = 0;
	size_t maxWidth = 0;

	while (std::getline(in, line))
	{
		if (row >= k_imgMaxSize)
			break;

		if (line.size() > k_imgMaxSize)
			line.resize(k_imgMaxSize);

		maxWidth = std::max(maxWidth, line.size());

		img.push_back(line);
		++row;
	}

	for (auto& r : img)
		r.resize(maxWidth, k_imgEmptyCh);

	return img;
}

void SaveImage(const Image& img, std::ostream& out)
{
	for (const auto& row : img)
		out << row << '\n';
}
