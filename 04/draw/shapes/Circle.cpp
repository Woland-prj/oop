#include "Circle.h"
#include "canvas/ICanvas.h"
#include <numbers>
#include <sstream>
#include <stdexcept>

Circle::Circle(Point center, double radius, uint32_t outlineColor, uint32_t fillColor)
	: SolidShapeBase(outlineColor, fillColor)
	, m_center(center)
	, m_radius(radius)
{
	if (radius <= 0.0)
		throw std::invalid_argument("Circle radius must be positive");
}

double Circle::GetArea() const
{
	return std::numbers::pi * m_radius * m_radius;
}

double Circle::GetPerimeter() const
{
	return 2.0 * std::numbers::pi * m_radius;
}

std::string Circle::ToString() const
{
	std::ostringstream oss;
	oss << "Circle: center(" << m_center.x << ", " << m_center.y << "), "
		<< "radius: " << m_radius << ", "
		<< "outline: " << ColorToString(m_outlineColor) << ", "
		<< "fill: " << ColorToString(m_fillColor) << ", "
		<< "area: " << GetArea() << ", perimeter: " << GetPerimeter();
	return oss.str();
}

Point Circle::GetCenter() const
{
	return m_center;
}

double Circle::GetRadius() const
{
	return m_radius;
}

void Circle::Draw(ICanvas& canvas) const
{
	canvas.FillCircle(m_center, m_radius, m_fillColor);
	canvas.DrawCircle(m_center, m_radius, m_outlineColor);
}
