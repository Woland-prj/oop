#include "ShapeParser.h"
#include "shapes/Circle.h"
#include "shapes/LineSegment.h"
#include "shapes/Rectangle.h"
#include "shapes/Triangle.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>

namespace ShapeParser::detail
{

constexpr char COMMENT_CH = '#';
constexpr char HEX_COLOR_PREFIX = '#';
constexpr size_t HEX_ARGB_LENGTH = 8;
constexpr size_t HEX_RGB_LENGTH = 6;
constexpr uint32_t HEX_ARGB_MASK = 0xFF000000;

std::string ToLower(std::string str)
{
	std::ranges::transform(str, str.begin(),
		[](const unsigned char c) { return std::tolower(c); });
	return str;
}

uint32_t ParseColor(const std::string& hex)
{
	std::string clean = hex;
	std::erase_if(clean, ::isspace);

	if (!clean.empty() && clean[0] == HEX_COLOR_PREFIX)
		clean = clean.substr(1);

	if (clean.empty() || clean.length() > HEX_ARGB_LENGTH)
		throw std::invalid_argument("Invalid color format: " + hex);

	auto value = static_cast<uint32_t>(std::stoul(clean, nullptr, 16));

	if (clean.length() == HEX_RGB_LENGTH)
		value |= HEX_ARGB_MASK;

	return value;
}

Point ParsePoint(const std::string& xStr, const std::string& yStr)
{
	try
	{
		return { std::stod(xStr), std::stod(yStr) };
	}
	catch (const std::exception& ex)
	{
		throw std::invalid_argument("Invalid point coordinates: (" + xStr + ", " + yStr + ")");
	}
}

std::unique_ptr<IShape> ParseRectangle(std::istringstream& iss)
{
	double x, y, w, h;
	std::string outlineHex, fillHex;

	if (!(iss >> x >> y >> w >> h >> outlineHex >> fillHex))
		throw std::invalid_argument("Rectangle: expected 6 parameters (x y w h outline fill)");

	return std::make_unique<Rectangle>(
		Point{ x, y }, w, h,
		ParseColor(outlineHex),
		ParseColor(fillHex));
}

std::unique_ptr<IShape> ParseCircle(std::istringstream& iss)
{
	double cx, cy, radius;
	std::string outlineHex, fillHex;

	if (!(iss >> cx >> cy >> radius >> outlineHex >> fillHex))
		throw std::invalid_argument("Circle: expected 5 parameters (cx cy r outline fill)");

	return std::make_unique<Circle>(
		Point{ cx, cy }, radius,
		ParseColor(outlineHex),
		ParseColor(fillHex));
}

std::unique_ptr<IShape> ParseTriangle(std::istringstream& iss)
{
	double x1, y1, x2, y2, x3, y3;
	std::string outlineHex, fillHex;

	if (!(iss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> outlineHex >> fillHex))
		throw std::invalid_argument("Triangle: expected 8 parameters (x1 y1 x2 y2 x3 y3 outline fill)");

	return std::make_unique<Triangle>(
		Point{ x1, y1 }, Point{ x2, y2 }, Point{ x3, y3 },
		ParseColor(outlineHex),
		ParseColor(fillHex));
}

std::unique_ptr<IShape> ParseLineSegment(std::istringstream& iss)
{
	double x1, y1, x2, y2;
	std::string outlineHex;

	if (!(iss >> x1 >> y1 >> x2 >> y2 >> outlineHex))
		throw std::invalid_argument("LineSegment: expected 5 parameters (x1 y1 x2 y2 outline)");

	return std::make_unique<LineSegment>(
		Point{ x1, y1 }, Point{ x2, y2 },
		ParseColor(outlineHex));
}

const DispatcherMap& GetHandlers()
{
	static const DispatcherMap handlers = {
		{ "rectangle", ParseRectangle },
		{ "circle", ParseCircle },
		{ "triangle", ParseTriangle },
		{ "line", ParseLineSegment },
		{ "linesegment", ParseLineSegment }
	};
	return handlers;
}

std::string GetAvailableShapeTypes()
{
	const auto& handlers = GetHandlers();
	std::string result;
	for (auto it = handlers.begin(); it != handlers.end(); ++it)
	{
		if (it != handlers.begin())
			result += ", ";
		result += it->first;
	}
	return result;
}

} // namespace ShapeParser::detail

namespace ShapeParser
{

std::unique_ptr<IShape> Parse(const std::string& line)
{
	std::istringstream iss(line);
	std::string type;

	if (!(iss >> type))
		throw std::invalid_argument("Empty line or missing shape type");

	type = detail::ToLower(type);

	const auto& handlers = detail::GetHandlers();
	const auto it = handlers.find(type);

	if (it == handlers.end())
		throw std::runtime_error("Unknown shape type: '" + type + "'. Available: " + detail::GetAvailableShapeTypes());

	return it->second(iss);
}

std::vector<std::unique_ptr<IShape>> ParseAll(std::istream& input)
{
	std::vector<std::unique_ptr<IShape>> shapes;
	std::string line;
	size_t lineNumber = 0;

	while (std::getline(input, line))
	{
		++lineNumber;

		const auto start = line.find_first_not_of(" \t\r\n");
		if (start == std::string::npos)
			continue;
		const auto end = line.find_last_not_of(" \t\r\n");
		line = line.substr(start, end - start + 1);

		if (line.empty() || line[0] == detail::COMMENT_CH)
			continue;

		try
		{
			shapes.push_back(Parse(line));
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error("Line " + std::to_string(lineNumber) + ": " + e.what());
		}
	}

	return shapes;
}

} // namespace ShapeParser