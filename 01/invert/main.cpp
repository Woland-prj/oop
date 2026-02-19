#include "matrix/Matrix.hpp"
#include <iostream>

int main()
{
	const Matrix m({
		{ 1, 2, 3, 5, 8 },
		{ 2, 11, 6, 7, 12 },
		{ 1, 15, 34, 8, 13 },
		{ 7, 12, 5, 12.67, 1.34 },
		{ 123, 1.28, 5.17, 18.9, 0 }
	});

	std::cout << m.Invert();
}