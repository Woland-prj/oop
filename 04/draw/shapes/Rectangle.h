#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "canvas/ICanvasDrawable.h"
#include "basic_types/Point.h"
#include "SolidShapeBase.h"

class Rectangle final : public SolidShapeBase
	, public ICanvasDrawable
{
public:
	Rectangle(Point topLeft, double width, double height, uint32_t outlineColor, uint32_t fillColor);
	~Rectangle() override = default;

	double GetArea() const override;
	double GetPerimeter() const override;
	std::string ToString() const override;
	Point GetTopLeft() const;
	double GetWidth() const;
	double GetHeight() const;
	Point GetBottomRight() const;
	void Draw(ICanvas& canvas) const override;

private:
	Point m_topLeft;
	double m_width;
	double m_height;
};

#endif // RECTANGLE_H
