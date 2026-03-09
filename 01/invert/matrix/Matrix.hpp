#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <array>
#include <cstddef>

template <size_t N>
class Matrix
{
public:
	Matrix();
	explicit Matrix(const double (&arr)[N][N]);
	explicit Matrix(std::istream& in);

	double& operator()(size_t row, size_t col);
	double operator()(size_t row, size_t col) const;

	Matrix operator*(double n) const;
	double Det() const;
	static constexpr size_t Size() noexcept;
	Matrix<N - 1> Minor(size_t row, size_t col) const;
	Matrix Cofactor() const;
	Matrix Transpose() const;
	Matrix Invert() const;

private:
	double DetGaussian();
	void SwapRows(size_t r1, size_t r2);

	std::array<double, N * N> data;
};

template <size_t N>
std::ostream& operator<<(std::ostream& out, const Matrix<N>& m);

#include "Matrix.tpp"

#endif // MATRIX_HPP
