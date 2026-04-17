#include "ShapeAnalyzer.h"
#include <algorithm>

namespace ShapeAnalyzer {

const IShape* FindMaxArea(const std::vector<std::unique_ptr<IShape>>& shapes)
{
	if (shapes.empty())
		return nullptr;

	return std::ranges::max_element(shapes,
		[](const auto& a, const auto& b) {
			return a->GetArea() < b->GetArea();
		})
		->get();
}

const IShape* FindMinPerimeter(const std::vector<std::unique_ptr<IShape>>& shapes)
{
	if (shapes.empty())
		return nullptr;

	return std::ranges::min_element(shapes,
		[](const auto& a, const auto& b) {
			return a->GetPerimeter() < b->GetPerimeter();
		})
		->get();
}

} // namespace ShapeAnalyzer