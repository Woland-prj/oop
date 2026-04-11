#ifndef OPERATION_H
#define OPERATION_H
#include <cmath>
#include <optional>

enum class Operation
{
	Add,
	Sub,
	Mul,
	Div
};

constexpr double DOUBLE_EPSILON = 1e-12;

inline std::optional<double> ApplyOperation(Operation op, double left, double right)
{
	if (std::isnan(left) || std::isnan(right))
		return std::nullopt;

	switch (op)
	{
	case Operation::Add:
		return left + right;
	case Operation::Sub:
		return left - right;
	case Operation::Mul:
		return left * right;
	case Operation::Div:
		if (right <= DOUBLE_EPSILON)
			return std::nullopt;
		return left / right;
	}
}

#endif // OPERATION_H
