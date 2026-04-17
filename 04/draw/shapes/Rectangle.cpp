#include "Rectangle.h"
#include <canvas/ICanvas.h>
#include <sstream>
#include <vector>

Rectangle::Rectangle(Point topLeft, double width, double height, uint32_t outlineColor, uint32_t fillColor)
	: SolidShapeBase(outlineColor, fillColor)
	, m_topLeft(topLeft)
	, m_width(width)
	, m_height(height)
{
	if (width <= 0 || height <= 0)
		throw std::invalid_argument("Rectangle dimensions must be positive");
}

double Rectangle::GetArea() const
{
	return m_width * m_height;
}

double Rectangle::GetPerimeter() const
{
	return 2 * (m_width + m_height);
}

std::string Rectangle::ToString() const
{
	std::ostringstream oss;
	oss << "Rectangle: topLeft(" << m_topLeft.x << ", " << m_topLeft.y << "), "
		<< "size " << m_width << "x" << m_height << ", "
		<< "outline: " << ColorToString(m_outlineColor) << ", "
		<< "fill: " << ColorToString(m_fillColor) << ", "
		<< "area: " << GetArea() << ", perimeter: " << GetPerimeter();
	return oss.str();
}

Point Rectangle::GetTopLeft() const
{
	return m_topLeft;
}

double Rectangle::GetWidth() const
{
	return m_width;
}
double Rectangle::GetHeight() const
{
	return m_height;
}

Point Rectangle::GetBottomRight() const
{
	return { m_topLeft.x + m_width, m_topLeft.y + m_height };
}

void Rectangle::Draw(ICanvas& canvas) const
{
	const std::vector<Point> points = {
		m_topLeft,
		{ m_topLeft.x + m_width, m_topLeft.y },
		{ m_topLeft.x + m_width, m_topLeft.y + m_height },
		{ m_topLeft.x, m_topLeft.y + m_height }
	};
	canvas.FillPolygon(points, m_fillColor);
	for (size_t i = 0; i < points.size(); ++i)
	{
		canvas.DrawLine(points[i], points[(i + 1) % points.size()], m_outlineColor);
	}
}
