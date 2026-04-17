#ifndef ISHAPE_H
#define ISHAPE_H
#include <string>

class IShape
{
public:
	virtual ~IShape() = default;

	virtual double GetArea() const = 0;
	virtual double GetPerimeter() const = 0;
	virtual std::string ToString() const = 0;
	virtual uint32_t GetOutlineColor() const = 0;
};

#endif // ISHAPE_H
