#ifndef SHAPE_PARSER_H
#define SHAPE_PARSER_H

#include "basic_types/IShape.h"
#include "basic_types/Point.h"
#include <functional>
#include <istream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace ShapeParser
{

std::unique_ptr<IShape> Parse(const std::string& line);

std::vector<std::unique_ptr<IShape>> ParseAll(std::istream& input);

namespace detail
{

using HandlerFunc = std::function<std::unique_ptr<IShape>(std::istringstream&)>;
using DispatcherMap = std::unordered_map<std::string, HandlerFunc>;

const DispatcherMap& GetHandlers();

std::unique_ptr<IShape> ParseRectangle(std::istringstream& iss);
std::unique_ptr<IShape> ParseCircle(std::istringstream& iss);
std::unique_ptr<IShape> ParseTriangle(std::istringstream& iss);
std::unique_ptr<IShape> ParseLineSegment(std::istringstream& iss);

uint32_t ParseColor(const std::string& hex);
Point ParsePoint(const std::string& xStr, const std::string& yStr);
std::string ToLower(std::string str);
std::string GetAvailableShapeTypes();

} // namespace detail

} // namespace ShapeParser

#endif // SHAPE_PARSER_H
