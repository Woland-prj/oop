#ifndef POINT_H
#define POINT_H

#include <cmath>

struct Point
{
	double x;
	double y;

	bool operator==(const Point& other) const {
		constexpr double EPS = 1e-9;
		return std::abs(x - other.x) < EPS && std::abs(y - other.y) < EPS;
	}
};

#endif // POINT_H
