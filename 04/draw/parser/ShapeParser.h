#ifndef SHAPE_PARSER_H
#define SHAPE_PARSER_H

#include "basic_types/IShape.h"
#include "basic_types/Point.h"
#include "canvas/ICanvasDrawable.h"
#include <functional>
#include <istream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace ShapeParser
{

using ShapePtr = std::shared_ptr<IShape>;
using DrawablePtr = std::shared_ptr<ICanvasDrawable>;

using ShapeVec = std::vector<ShapePtr>;
using DrawableVec = std::vector<DrawablePtr>;

std::pair<ShapePtr, DrawablePtr> Parse(const std::string& line);
std::pair<ShapeVec, DrawableVec> ParseAll(std::istream& input);

namespace detail
{

using HandlerFunc = std::function<std::pair<ShapePtr, DrawablePtr>(std::istringstream&)>;
using DispatcherMap = std::unordered_map<std::string, HandlerFunc>;

const DispatcherMap& GetHandlers();

std::pair<ShapePtr, DrawablePtr> ParseRectangle(std::istringstream& iss);
std::pair<ShapePtr, DrawablePtr> ParseCircle(std::istringstream& iss);
std::pair<ShapePtr, DrawablePtr> ParseTriangle(std::istringstream& iss);
std::pair<ShapePtr, DrawablePtr> ParseLineSegment(std::istringstream& iss);

uint32_t ParseColor(const std::string& hex);
Point ParsePoint(const std::string& xStr, const std::string& yStr);
std::string ToLower(std::string str);
std::string GetAvailableShapeTypes();

} // namespace detail

} // namespace ShapeParser

#endif // SHAPE_PARSER_H
