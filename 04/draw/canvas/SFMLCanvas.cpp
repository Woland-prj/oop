#include "SFMLCanvas.h"
#include <array>
#include <cmath>
#include <sstream>

SFMLCanvas::SFMLCanvas(sf::RenderTarget& target)
	: m_target(target)
{
}

float SFMLCanvas::SafeDoubleToFloat(double value, const std::string& context)
{
	if (std::isnan(value))
		throw RenderError("NaN value in " + context);
	if (std::isinf(value))
		throw RenderError("Infinite value in " + context);

	constexpr auto floatMax = static_cast<double>(std::numeric_limits<float>::max());
	if (std::abs(value) > floatMax)
	{
		std::ostringstream oss;
		oss << "Value " << value << " out of float range in " << context;
		throw RenderError(oss.str());
	}

	return static_cast<float>(value);
}

sf::Color SFMLCanvas::ToSFMLColor(uint32_t color)
{
	// 0xAARRGGBB -> sf::Color(R, G, B, A)
	return {
		static_cast<std::uint8_t>((color >> 16) & 0xFF), // R
		static_cast<std::uint8_t>((color >> 8) & 0xFF), // G
		static_cast<std::uint8_t>(color & 0xFF), // B
		static_cast<std::uint8_t>((color >> 24) & 0xFF) // A
	};
}

sf::Vector2f SFMLCanvas::ToScreen(const Point& p) const
{
	const double screenX = m_offsetX + p.x * m_scale;
	const double screenY = m_offsetY + p.y * m_scale;

	if (std::abs(screenX) > MAX_SAFE_COORD || std::abs(screenY) > MAX_SAFE_COORD)
	{
		std::ostringstream oss;
		oss << "Point (" << p.x << ", " << p.y << ") transformed to ("
			<< screenX << ", " << screenY << ") exceeds safe bounds";
		throw OutOfBoundsError(oss.str());
	}

	return {
		SafeDoubleToFloat(screenX, "coordinate x"),
		SafeDoubleToFloat(screenY, "coordinate y")
	};
}

float SFMLCanvas::ValidateRadius(double radius) const
{
	if (radius <= 0)
	{
		throw RenderError("Radius must be positive");
	}

	const double transformedRadius = radius * m_scale;
	if (transformedRadius > MAX_SAFE_COORD)
	{
		std::ostringstream oss;
		oss << "Radius " << transformedRadius << " exceeds maximum allowed";
		throw OutOfBoundsError(oss.str());
	}

	return SafeDoubleToFloat(transformedRadius, "radius");
}

bool SFMLCanvas::IsValidForRendering(const sf::Vector2f& screenPoint)
{
	return std::abs(screenPoint.x) < MAX_SAFE_COORD && std::abs(screenPoint.y) < MAX_SAFE_COORD;
}

bool SFMLCanvas::IsPointVisible(const Point& p) const
{
	try
	{
		const auto screen = ToScreen(p);
		return IsValidForRendering(screen);
	}
	catch (...)
	{
		return false;
	}
}

void SFMLCanvas::DrawLine(const Point& from, const Point& to, uint32_t lineColor)
{
	const auto screenFrom = ToScreen(from);
	const auto screenTo = ToScreen(to);

	if (!IsValidForRendering(screenFrom) || !IsValidForRendering(screenTo))
		return;

	const std::array<sf::Vertex, 2> line = { { sf::Vertex(screenFrom, ToSFMLColor(lineColor)),
		sf::Vertex(screenTo, ToSFMLColor(lineColor)) } };

	m_target.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
}

void SFMLCanvas::FillPolygon(const std::vector<Point>& points, uint32_t fillColor)
{
	if (points.size() < 3)
		throw RenderError("Polygon requires at least 3 points");

	sf::ConvexShape polygon(points.size());
	polygon.setFillColor(ToSFMLColor(fillColor));

	for (size_t i = 0; i < points.size(); ++i)
	{
		auto screenPoint = ToScreen(points[i]);
		if (!IsValidForRendering(screenPoint))
		{
			std::ostringstream oss;
			oss << "Polygon vertex " << i << " at (" << points[i].x << ", " << points[i].y
				<< ") out of rendering bounds";
			throw OutOfBoundsError(oss.str());
		}
		polygon.setPoint(i, screenPoint);
	}

	m_target.draw(polygon);
}

void SFMLCanvas::DrawCircle(const Point& center, double radius, uint32_t lineColor)
{
	const float screenRadius = ValidateRadius(radius);
	const auto screenCenter = ToScreen(center);

	if (!IsValidForRendering(screenCenter))
	{
		std::ostringstream oss;
		oss << "Circle center (" << center.x << ", " << center.y << ") out of rendering bounds";
		throw OutOfBoundsError(oss.str());
	}

	sf::CircleShape circle(screenRadius);
	circle.setPosition(screenCenter);
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineColor(ToSFMLColor(lineColor));
	circle.setOutlineThickness(1.0f);
	circle.setOrigin(sf::Vector2f{ screenRadius, screenRadius });

	m_target.draw(circle);
}

void SFMLCanvas::FillCircle(const Point& center, double radius, uint32_t fillColor)
{
	const float screenRadius = ValidateRadius(radius);
	const auto screenCenter = ToScreen(center);

	if (!IsValidForRendering(screenCenter))
	{
		std::ostringstream oss;
		oss << "Circle center (" << center.x << ", " << center.y << ") out of rendering bounds";
		throw OutOfBoundsError(oss.str());
	}

	sf::CircleShape circle(screenRadius);
	circle.setPosition(screenCenter);
	circle.setFillColor(ToSFMLColor(fillColor));
	circle.setOrigin(sf::Vector2f{ screenRadius, screenRadius });

	m_target.draw(circle);
}

void SFMLCanvas::SetScale(double scale)
{
	if (scale <= 0.0)
		throw std::invalid_argument("Scale must be positive");
	if (scale > MAX_SAFE_COORD)
		throw std::invalid_argument("Scale too large");

	m_scale = scale;
}

void SFMLCanvas::SetOffset(double offsetX, double offsetY)
{
	if (std::abs(offsetX) > MAX_SAFE_COORD || std::abs(offsetY) > MAX_SAFE_COORD)
		throw std::invalid_argument("Offset exceeds safe bounds");

	m_offsetX = offsetX;
	m_offsetY = offsetY;
}

void SFMLCanvas::Clear(uint32_t backgroundColor) const
{
	m_target.clear(ToSFMLColor(backgroundColor));
}