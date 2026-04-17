#define CATCH_CONFIG_MAIN
#include "analyzer/ShapeAnalyzer.h"
#include "canvas/ICanvas.h"
#include "catch2/matchers/catch_matchers_string.hpp"
#include "fakeit.hpp"
#include "parser/ShapeParser.h"
#include "shapes/Circle.h"
#include "shapes/LineSegment.h"
#include "shapes/Rectangle.h"
#include "shapes/Triangle.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <memory>
#include <numbers>
#include <sstream>
#include <vector>

TEST_CASE("Point is a simple aggregate", "[basic]")
{
	Point p{ 1.5, -2.3 };
	CHECK(p.x == 1.5);
	CHECK(p.y == -2.3);
}

TEST_CASE("Rectangle: construction and getters", "[rectangle]")
{
	Rectangle rect({ 10, 20 }, 30, 40, 0xFF0000, 0x00FF00);

	CHECK(rect.GetTopLeft().x == 10.0);
	CHECK(rect.GetTopLeft().y == 20.0);
	CHECK(rect.GetWidth() == 30.0);
	CHECK(rect.GetHeight() == 40.0);
	CHECK(rect.GetBottomRight().x == 40.0);
	CHECK(rect.GetBottomRight().y == 60.0);
	CHECK(rect.GetOutlineColor() == 0xFF0000);
	CHECK(rect.GetFillColor() == 0x00FF00);
}

TEST_CASE("Rectangle: throws on invalid dimensions", "[rectangle]")
{
	CHECK_THROWS_AS(Rectangle({ 0, 0 }, -5, 10, 0, 0), std::invalid_argument);
	CHECK_THROWS_AS(Rectangle({ 0, 0 }, 10, 0, 0, 0), std::invalid_argument);
	CHECK_THROWS_AS(Rectangle({ 0, 0 }, 0, 10, 0, 0), std::invalid_argument);
}

TEST_CASE("Rectangle: area and perimeter", "[rectangle]")
{
	Rectangle rect({ 0, 0 }, 5, 7, 0, 0);
	CHECK(rect.GetArea() == 35.0);
	CHECK(rect.GetPerimeter() == 24.0);
}

TEST_CASE("Circle: construction and getters", "[circle]")
{
	Circle circle({ 100, 200 }, 15.5, 0x0000FF, 0xFFFF00);

	CHECK(circle.GetCenter().x == 100.0);
	CHECK(circle.GetCenter().y == 200.0);
	CHECK(circle.GetRadius() == 15.5);
	CHECK(circle.GetOutlineColor() == 0x0000FF);
	CHECK(circle.GetFillColor() == 0xFFFF00);
}

TEST_CASE("Circle: throws on invalid radius", "[circle]")
{
	CHECK_THROWS_AS(Circle({ 0, 0 }, 0, 0, 0), std::invalid_argument);
	CHECK_THROWS_AS(Circle({ 0, 0 }, -1, 0, 0), std::invalid_argument);
}

TEST_CASE("Circle: area and perimeter", "[circle]")
{
	Circle circle({ 0, 0 }, 10, 0, 0);
	CHECK_THAT(circle.GetArea(), Catch::Matchers::WithinRel(std::numbers::pi * 100, 1e-9));
	CHECK_THAT(circle.GetPerimeter(), Catch::Matchers::WithinRel(2 * std::numbers::pi * 10, 1e-9));
}

TEST_CASE("Triangle: construction and getters", "[triangle]")
{
	Triangle tri({ 0, 0 }, { 10, 0 }, { 5, 10 }, 0xFF00FF, 0x00FFFF);

	CHECK(tri.GetVertex1().x == 0.0);
	CHECK(tri.GetVertex2().x == 10.0);
	CHECK(tri.GetVertex3().y == 10.0);
	CHECK(tri.GetOutlineColor() == 0xFF00FF);
	CHECK(tri.GetFillColor() == 0x00FFFF);
}

TEST_CASE("Triangle: throws on collinear points", "[triangle]")
{
	CHECK_THROWS_AS(Triangle({ 0, 0 }, { 5, 5 }, { 10, 10 }, 0, 0), std::invalid_argument);
}

TEST_CASE("Triangle: area via shoelace formula", "[triangle]")
{
	Triangle tri({ 0, 0 }, { 3, 0 }, { 0, 4 }, 0, 0);
	CHECK(tri.GetArea() == 6.0);
	CHECK_THAT(tri.GetPerimeter(), Catch::Matchers::WithinRel(3 + 4 + 5, 1e-9));
}

TEST_CASE("LineSegment: construction and getters", "[line]")
{
	LineSegment line({ 1, 2 }, { 4, 6 }, 0xFFFFFF);

	CHECK(line.GetStartPoint().x == 1.0);
	CHECK(line.GetEndPoint().y == 6.0);
	CHECK(line.GetOutlineColor() == 0xFFFFFF);
}

TEST_CASE("LineSegment: area is zero, perimeter is length", "[line]")
{
	LineSegment line({ 0, 0 }, { 3, 4 }, 0);
	CHECK(line.GetArea() == 0.0);
	CHECK(line.GetPerimeter() == 5.0);
}

TEST_CASE("Rectangle::ToString contains expected info", "[rectangle][tostring]")
{
	Rectangle rect({ 1.5, 2.5 }, 10, 20, 0xFF0000, 0x00FF00);
	std::string str = rect.ToString();

	CHECK_THAT(str, Catch::Matchers::ContainsSubstring("Rectangle"));
	CHECK_THAT(str, Catch::Matchers::ContainsSubstring("topLeft(1.5, 2.5)"));
	CHECK_THAT(str, Catch::Matchers::ContainsSubstring("size 10x20"));
	CHECK_THAT(str, Catch::Matchers::ContainsSubstring("#00ff0000"));
	CHECK_THAT(str, Catch::Matchers::ContainsSubstring("#0000ff00"));
	CHECK_THAT(str, Catch::Matchers::ContainsSubstring("area: 200"));
	CHECK_THAT(str, Catch::Matchers::ContainsSubstring("perimeter: 60"));
}

TEST_CASE("Circle::ToString contains expected info", "[circle][tostring]")
{
	Circle circle({ 100, 200 }, 15, 0x0000FF, 0xFFFF00);
	std::string str = circle.ToString();

	CHECK_THAT(str, Catch::Matchers::ContainsSubstring("Circle"));
	CHECK_THAT(str, Catch::Matchers::ContainsSubstring("center(100, 200)"));
	CHECK_THAT(str, Catch::Matchers::ContainsSubstring("radius: 15"));
	CHECK_THAT(str, Catch::Matchers::ContainsSubstring("area:"));
}

TEST_CASE("Rectangle::Draw calls FillPolygon and 4 DrawLine", "[rectangle][draw]")
{
	const Rectangle rect({ 10, 20 }, 30, 40, 0xFF0000, 0x00FF00);

	fakeit::Mock<ICanvas> mockCanvas;
	fakeit::Fake(Method(mockCanvas, DrawLine));
	fakeit::Fake(Method(mockCanvas, FillPolygon));

	ICanvas& canvas = mockCanvas.get();
	rect.Draw(canvas);

	fakeit::Verify(Method(mockCanvas, FillPolygon)
					   .Using(std::vector<Point>{
								  Point{ 10, 20 },
								  Point{ 40, 20 },
								  Point{ 40, 60 },
								  Point{ 10, 60 } },
						   0x00FF00))
		.Once();

	fakeit::Verify(Method(mockCanvas, DrawLine)
					   .Using(fakeit::_, fakeit::_, 0xFF0000))
		.Exactly(4);
}

TEST_CASE("Circle::Draw calls FillCircle then DrawCircle", "[circle][draw]")
{
	const Circle circle({ 50, 100 }, 25, 0x0000FF, 0xFFFF00);

	fakeit::Mock<ICanvas> mockCanvas;
	fakeit::Fake(Method(mockCanvas, FillCircle));
	fakeit::Fake(Method(mockCanvas, DrawCircle));

	ICanvas& canvas = mockCanvas.get();
	circle.Draw(canvas);

	fakeit::Verify(Method(mockCanvas, FillCircle).Using(Point{ 50, 100 }, 25.0, 0xFFFF00)).Once();
	fakeit::Verify(Method(mockCanvas, DrawCircle).Using(Point{ 50, 100 }, 25.0, 0x0000FF)).Once();
}

TEST_CASE("Triangle::Draw calls FillPolygon and 3 DrawLine", "[triangle][draw]")
{
	const Triangle tri({ 0, 0 }, { 10, 0 }, { 5, 10 }, 0xFF00FF, 0x00FFFF);

	fakeit::Mock<ICanvas> mockCanvas;
	fakeit::Fake(Method(mockCanvas, DrawLine));
	fakeit::Fake(Method(mockCanvas, FillPolygon));

	ICanvas& canvas = mockCanvas.get();
	tri.Draw(canvas);

	fakeit::Verify(Method(mockCanvas, FillPolygon).Using(std::vector<Point>{ Point{ 0, 0 }, Point{ 10, 0 }, Point{ 5, 10 } }, 0x00FFFF)).Once();

	fakeit::Verify(Method(mockCanvas, DrawLine).Using(fakeit::_, fakeit::_, 0xFF00FF)).Exactly(3);
}

TEST_CASE("LineSegment::Draw calls DrawLine once", "[line][draw]")
{
	const LineSegment line({ 1, 2 }, { 3, 4 }, 0xFFFFFF);

	fakeit::Mock<ICanvas> mockCanvas;
	fakeit::Fake(Method(mockCanvas, DrawLine));

	ICanvas& canvas = mockCanvas.get();
	line.Draw(canvas);

	fakeit::Verify(Method(mockCanvas, DrawLine).Using(Point{ 1, 2 }, Point{ 3, 4 }, 0xFFFFFF)).Once();
}

TEST_CASE("ShapeParser::ParseColor handles various formats", "[parser]")
{
	auto rect = ShapeParser::Parse("rectangle 0 0 10 10 ff0000 00ff00");
	auto* r = dynamic_cast<Rectangle*>(rect.get());
	REQUIRE(r != nullptr);
	CHECK(r->GetOutlineColor() == 0xFFFF0000);
	CHECK(r->GetFillColor() == 0xFF00FF00);

	auto rect2 = ShapeParser::Parse("rectangle 0 0 10 10 #ff0000 #00ff00");
	auto* r2 = dynamic_cast<Rectangle*>(rect2.get());
	REQUIRE(r2 != nullptr);
	CHECK(r2->GetOutlineColor() == 0xFFFF0000);
}

TEST_CASE("ShapeParser::ParseAll skips comments and empty lines", "[parser]")
{
	std::istringstream input(R"(
# Это комментарий
rectangle 0 0 10 10 ff0000 00ff00

   # Ещё комментарий
circle 50 50 5 00ff00 ff00ff

)");

	auto shapes = ShapeParser::ParseAll(input);

	REQUIRE(shapes.size() == 2);
	CHECK(dynamic_cast<Rectangle*>(shapes[0].get()) != nullptr);
	CHECK(dynamic_cast<Circle*>(shapes[1].get()) != nullptr);
}

TEST_CASE("FindMaxArea returns shape with largest area", "[analyzer]")
{
	std::vector<std::unique_ptr<IShape>> shapes;

	shapes.push_back(std::make_unique<Rectangle>(Point{ 0, 0 }, 2, 3, 0, 0));
	shapes.push_back(std::make_unique<Circle>(Point{ 0, 0 }, 10, 0, 0));
	shapes.push_back(std::make_unique<Rectangle>(Point{ 0, 0 }, 1, 1, 0, 0));

	const IShape* result = ShapeAnalyzer::FindMaxArea(shapes);

	REQUIRE(result != nullptr);
	CHECK_THAT(result->GetArea(), Catch::Matchers::WithinRel(std::numbers::pi * 100, 1e-9));
}

TEST_CASE("FindMinPerimeter returns shape with smallest perimeter", "[analyzer]")
{
	std::vector<std::unique_ptr<IShape>> shapes;

	shapes.push_back(std::make_unique<Rectangle>(Point{ 0, 0 }, 10, 10, 0, 0));
	shapes.push_back(std::make_unique<LineSegment>(Point{ 0, 0 }, Point{ 1, 0 }, 0));
	shapes.push_back(std::make_unique<Circle>(Point{ 0, 0 }, 5, 0, 0));

	const IShape* result = ShapeAnalyzer::FindMinPerimeter(shapes);

	REQUIRE(result != nullptr);
	CHECK(result->GetPerimeter() == 1.0);
}

TEST_CASE("FindMaxArea/MinPerimeter return nullptr for empty vector", "[analyzer]")
{
	const std::vector<std::unique_ptr<IShape>> empty;

	CHECK(ShapeAnalyzer::FindMaxArea(empty) == nullptr);
	CHECK(ShapeAnalyzer::FindMinPerimeter(empty) == nullptr);
}
