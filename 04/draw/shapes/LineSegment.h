#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "canvas/ICanvasDrawable.h"
#include "basic_types/Point.h"
#include "ShapeBase.h"

class LineSegment final : public ShapeBase
	, public ICanvasDrawable
{
public:
	LineSegment(Point start, Point end, uint32_t outlineColor);
	~LineSegment() override = default;

	double GetArea() const override;
	double GetPerimeter() const override;
	std::string ToString() const override;

	Point GetStartPoint() const;
	Point GetEndPoint() const;

	void Draw(ICanvas& canvas) const override;

private:
	Point m_start;
	Point m_end;

	static double Distance(const Point& a, const Point& b);
};

#endif // LINESEGMENT_H
