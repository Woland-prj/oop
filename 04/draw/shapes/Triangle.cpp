#include "Triangle.h"
#include "canvas/ICanvas.h"
#include <cmath>
#include <sstream>
#include <stdexcept>

Triangle::Triangle(Point v1, Point v2, Point v3, uint32_t outlineColor, uint32_t fillColor)
	: SolidShapeBase(outlineColor, fillColor)
	, m_v1(v1)
	, m_v2(v2)
	, m_v3(v3)
{
	if (GetArea() <= 0.0)
		throw std::invalid_argument("Triangle vertices are collinear");
}

double Triangle::Distance(const Point& a, const Point& b)
{
	double dx = b.x - a.x;
	double dy = b.y - a.y;
	return std::sqrt(dx * dx + dy * dy);
}

double Triangle::GetArea() const
{
	return std::abs((m_v2.x - m_v1.x) * (m_v3.y - m_v1.y) - (m_v3.x - m_v1.x) * (m_v2.y - m_v1.y)) / 2.0;
}

double Triangle::GetPerimeter() const
{
	return Distance(m_v1, m_v2) + Distance(m_v2, m_v3) + Distance(m_v3, m_v1);
}

std::string Triangle::ToString() const
{
	std::ostringstream oss;
	oss << "Triangle: v1(" << m_v1.x << ", " << m_v1.y << "), "
		<< "v2(" << m_v2.x << ", " << m_v2.y << "), "
		<< "v3(" << m_v3.x << ", " << m_v3.y << "), "
		<< "outline: " << ColorToString(m_outlineColor) << ", "
		<< "fill: " << ColorToString(m_fillColor) << ", "
		<< "area: " << GetArea() << ", perimeter: " << GetPerimeter();
	return oss.str();
}

Point Triangle::GetVertex1() const
{
	return m_v1;
}

Point Triangle::GetVertex2() const
{
	return m_v2;
}

Point Triangle::GetVertex3() const
{
	return m_v3;
}

void Triangle::Draw(ICanvas& canvas) const
{
	std::vector<Point> points = { m_v1, m_v2, m_v3 };
	canvas.FillPolygon(points, m_fillColor);

	canvas.DrawLine(m_v1, m_v2, m_outlineColor);
	canvas.DrawLine(m_v2, m_v3, m_outlineColor);
	canvas.DrawLine(m_v3, m_v1, m_outlineColor);
}
