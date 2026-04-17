#include "LineSegment.h"
#include "canvas/ICanvas.h"
#include <cmath>
#include <sstream>

LineSegment::LineSegment(Point start, Point end, uint32_t outlineColor)
	: ShapeBase(outlineColor)
	, m_start(start)
	, m_end(end)
{
}

double LineSegment::Distance(const Point& a, const Point& b)
{
	double dx = b.x - a.x;
	double dy = b.y - a.y;
	return std::sqrt(dx * dx + dy * dy);
}

double LineSegment::GetArea() const
{
	return 0.0;
}

double LineSegment::GetPerimeter() const
{
	return Distance(m_start, m_end);
}

std::string LineSegment::ToString() const
{
	std::ostringstream oss;
	double length = GetPerimeter();
	oss << "LineSegment: start(" << m_start.x << ", " << m_start.y << "), "
		<< "end(" << m_end.x << ", " << m_end.y << "), "
		<< "length: " << length << ", "
		<< "outline: " << ColorToString(m_outlineColor) << ", "
		<< "area: " << GetArea() << ", perimeter: " << GetPerimeter();
	return oss.str();
}

Point LineSegment::GetStartPoint() const
{
	return m_start;
}

Point LineSegment::GetEndPoint() const
{
	return m_end;
}

void LineSegment::Draw(ICanvas& canvas) const
{
	canvas.DrawLine(m_start, m_end, m_outlineColor);
}
