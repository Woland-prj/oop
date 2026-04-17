#ifndef CIRCLE_H
#define CIRCLE_H

#include "canvas/ICanvasDrawable.h"
#include "basic_types/Point.h"
#include "SolidShapeBase.h"

class Circle final : public SolidShapeBase
	, public ICanvasDrawable
{
public:
	Circle(Point center, double radius, uint32_t outlineColor, uint32_t fillColor);
	~Circle() override = default;

	double GetArea() const override;
	double GetPerimeter() const override;
	std::string ToString() const override;

	Point GetCenter() const;
	double GetRadius() const;

	void Draw(ICanvas& canvas) const override;

private:
	Point m_center;
	double m_radius;
};

#endif // CIRCLE_H
