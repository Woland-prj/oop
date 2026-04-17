#ifndef SOLIDSHAPEBASE_H
#define SOLIDSHAPEBASE_H

#include "basic_types/ISolidShape.h"
#include "ShapeBase.h"

class SolidShapeBase : public ShapeBase
	, public ISolidShape
{
public:
	uint32_t GetFillColor() const final;

protected:
	SolidShapeBase(uint32_t outlineColor, uint32_t fillColor);

	uint32_t m_fillColor;
};

#endif // SOLIDSHAPEBASE_H
