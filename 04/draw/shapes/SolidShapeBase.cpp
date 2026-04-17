#include "SolidShapeBase.h"

SolidShapeBase::SolidShapeBase(uint32_t outlineColor, uint32_t fillColor)
	: ShapeBase(outlineColor)
	, m_fillColor(fillColor)
{
}

uint32_t SolidShapeBase::GetFillColor() const
{
	return m_fillColor;
}
