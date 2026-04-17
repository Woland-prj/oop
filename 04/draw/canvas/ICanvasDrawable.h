#ifndef ICANVASDRAWABLE_H
#define ICANVASDRAWABLE_H

class ICanvas;

class ICanvasDrawable
{
public:
	virtual ~ICanvasDrawable() = default;

	virtual void Draw(ICanvas& canvas) const = 0;
};

#endif // ICANVASDRAWABLE_H
