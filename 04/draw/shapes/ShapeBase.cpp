#include "ShapeBase.h"
#include <iomanip>
#include <sstream>

ShapeBase::ShapeBase(uint32_t outlineColor)
	: m_outlineColor(outlineColor)
{
}

uint32_t ShapeBase::GetOutlineColor() const
{
	return m_outlineColor;
}

std::string ShapeBase::ColorToString(uint32_t color)
{
	std::ostringstream oss;
	oss << "#" << std::hex << std::setfill('0') << std::setw(8) << color;
	return oss.str();
}
