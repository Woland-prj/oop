#include <cmath>
#include <stdexcept>
#include <cstddef>
#include <sstream>
#include <iomanip>

constexpr double k_epsilon = 1e-12;
constexpr std::size_t k_precision = 3;

template <size_t N>
Matrix<N>::Matrix() = default;

template <size_t N>
Matrix<N>::Matrix(const double (&arr)[N][N])
{
	for (size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < N; ++j)
			(*this)(i, j) = arr[i][j];
}

template <size_t N>
Matrix<N>::Matrix(std::istream& in)
{
	for (size_t i = 0; i < N; ++i)
	{
		for (size_t j = 0; j < N; ++j)
		{
			if (!(in >> (*this)(i, j)))
			{
				if (in.eof())
					throw std::runtime_error("Invalid matrix format");
				throw std::runtime_error("Invalid matrix");
			}
		}
	}

	double extra;
	if (in >> extra)
		throw std::runtime_error("Invalid matrix format");
}

template <size_t N>
constexpr size_t Matrix<N>::Size() noexcept
{
	return N;
}

template <size_t N>
double& Matrix<N>::operator()(const size_t row, const size_t col)
{
	if (row >= N || col >= N)
		throw std::out_of_range("Matrix index out of range");
	return data[row * N + col];
}

template <size_t N>
double Matrix<N>::operator()(const size_t row, const size_t col) const
{
	if (row >= N || col >= N)
		throw std::out_of_range("Matrix index out of range");
	return data[row * N + col];
}

template <size_t N>
Matrix<N> Matrix<N>::operator*(double n) const
{
	Matrix res;
	for (size_t j = 0; j < N; ++j)
	{
		for (size_t i = 0; i < N; ++i)
		{
			res(j, i) = (*this)(j, i) * n;
		}
	}
	return res;
}

template <size_t N>
double Matrix<N>::Det() const
{
	Matrix<N> tmp = *this;
	return tmp.DetGaussian();
}

template <size_t N>
Matrix<N - 1> Matrix<N>::Minor(const size_t row, const size_t col) const
{
	Matrix<N - 1> res;
	size_t nRow = 0;
	for (size_t i = 0; i < N; ++i)
	{
		if (i == row)
			continue;
		size_t nCol = 0;
		for (size_t j = 0; j < N; ++j)
		{
			if (j == col)
				continue;
			res(nRow, nCol) = (*this)(i, j);
			++nCol;
		}
		++nRow;
	}
	return res;
}

template <size_t N>
Matrix<N> Matrix<N>::Cofactor() const
{
	Matrix result;
	for (size_t row = 0; row < N; ++row)
	{
		for (size_t col = 0; col < N; ++col)
		{
			double s = ((row + col) % 2 == 0) ? 1.0 : -1.0;
			result(row, col) = s * Minor(row, col).Det();
		}
	}
	return result;
}

template <size_t N>
Matrix<N> Matrix<N>::Transpose() const
{
	Matrix result;
	for (size_t row = 0; row < N; ++row)
	{
		for (size_t col = 0; col < N; ++col)
		{
			result(row, col) = (*this)(col, row);
		}
	}
	return result;
}

template <size_t N>
Matrix<N> Matrix<N>::Invert() const
{
	const double det = (*this).Det();
	if (std::abs(det) < k_epsilon)
		throw std::domain_error("Non-invertible");
	return Cofactor().Transpose() * (1.0 / det);
}

template <size_t N>
double Matrix<N>::DetGaussian()
{
	double det = 1.0;
	int sign = 1;

	for (size_t i = 0; i < N; ++i) {
		size_t pivot = i;
		for (size_t row = i + 1; row < N; ++row) {
			if (std::abs((*this)(row, i)) > std::abs((*this)(pivot, i)))
				pivot = row;
		}

		if (std::abs((*this)(pivot, i)) < k_epsilon)
			return 0.0;

		if (pivot != i) {
			SwapRows(i, pivot);
			sign *= -1;
		}

		double pivotValue = (*this)(i, i);
		det *= pivotValue;

		for (size_t row = i + 1; row < N; ++row) {
			double factor = (*this)(row, i) / pivotValue;
			for (size_t col = i; col < N; ++col) {
				(*this)(row, col) -= factor * (*this)(i, col);
			}
		}
	}

	return det * sign;
}

template <size_t N>
void Matrix<N>::SwapRows(size_t r1, size_t r2)
{
	for (size_t j = 0; j < N; ++j)
		std::swap((*this)(r1, j), (*this)(r2, j));
}

template <size_t N>
std::ostream& operator<<(std::ostream& out, const Matrix<N>& m)
{
    out << std::fixed << std::setprecision(k_precision);
	for (size_t row = 0; row < N; ++row)
	{
		for (size_t col = 0; col < N; ++col)
		{
		    double value = m(row, col);

            if (std::abs(value) < k_epsilon) // Делать не при выводе, а при расчетах
                value = 0.0;

            out << value;
			if (col != N - 1)
				out << '\t';
		}
		out << '\n';
	}
	return out;
}
