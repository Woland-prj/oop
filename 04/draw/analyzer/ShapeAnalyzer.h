#ifndef SHAPE_ANALYZER_H
#define SHAPE_ANALYZER_H

#include "basic_types/IShape.h"
#include <memory>
#include <vector>

namespace ShapeAnalyzer
{

const IShape* FindMaxArea(const std::vector<std::shared_ptr<IShape>>& shapes);

const IShape* FindMinPerimeter(const std::vector<std::shared_ptr<IShape>>& shapes);

} // namespace ShapeAnalyzer

#endif // SHAPE_ANALYZER_H
