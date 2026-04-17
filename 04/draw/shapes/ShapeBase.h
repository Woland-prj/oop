#ifndef SHAPEBASE_H
#define SHAPEBASE_H
#include "basic_types/IShape.h"

#include <cstdint>

class ShapeBase : public virtual IShape
{
public:
	uint32_t GetOutlineColor() const final;

protected:
	explicit ShapeBase(uint32_t outlineColor);

	static std::string ColorToString(uint32_t color);

	uint32_t m_outlineColor;
};

#endif // SHAPEBASE_H
