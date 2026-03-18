#include "ExpressionParser.hpp"

#include <cctype>
#include <cstdint>
#include <stack>
#include <stdexcept>

struct Frame
{
	char op;
	int64_t acc;
};

void ApplyOp(Frame& f, int64_t v)
{
	if (f.op == '+')
		f.acc += v;
	else
		f.acc *= v;
}

// Разбить на отдельные функции обработки
int64_t EvalPrefixExpression(std::istream& input)
{
	std::stack<Frame> frames;
	char ch;

	while (input >> ch)
	{
		if (ch == '(')
		{
			if (!(input >> ch))
				throw std::runtime_error("Unexpected end of input after '('");

			if (ch != '+' && ch != '*')
				throw std::runtime_error(std::format("Expected '+' or '*' operator. Got {}", ch));

			int64_t identity = (ch == '+') ? 0 : 1;
			frames.push({ ch, identity });
		}
		else if (ch == ')')
		{
			if (frames.empty())
				throw std::runtime_error("Unmatched closing parenthesis");

			int64_t result = frames.top().acc;
			frames.pop();

			if (frames.empty())
			{
				char extra;
				if (input >> extra)
				{
					if (!std::isspace(static_cast<unsigned char>(extra)))
						throw std::runtime_error("Unexpected data after expression");
				}
				return result;
			}

			auto& parent = frames.top();
			ApplyOp(parent, result);
		}
		else if (ch == '-' || std::isdigit(static_cast<unsigned char>(ch)))
		{
			input.putback(ch);
			int64_t num;
			if (!(input >> num))
				throw std::runtime_error("Failed to parse number");

			if (frames.empty())
				throw std::runtime_error("Number outside of expression context");

			auto& frame = frames.top();
			ApplyOp(frame, num);
		}
		else if (!std::isspace(static_cast<unsigned char>(ch)))
		{
			throw std::runtime_error("Unexpected character in input");
		}
	}

	if (!frames.empty())
		throw std::runtime_error("Unclosed expression");

	throw std::runtime_error("Empty input");
}
