#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "canvas/ICanvasDrawable.h"
#include "basic_types/Point.h"
#include "SolidShapeBase.h"

class Triangle final : public SolidShapeBase
	, public ICanvasDrawable
{
public:
	Triangle(Point v1, Point v2, Point v3, uint32_t outlineColor, uint32_t fillColor);
	~Triangle() override = default;

	double GetArea() const override;
	double GetPerimeter() const override;
	std::string ToString() const override;

	Point GetVertex1() const;
	Point GetVertex2() const;
	Point GetVertex3() const;

	void Draw(ICanvas& canvas) const override;

private:
	Point m_v1, m_v2, m_v3;

	static double Distance(const Point& a, const Point& b);
};

#endif // TRIANGLE_H
