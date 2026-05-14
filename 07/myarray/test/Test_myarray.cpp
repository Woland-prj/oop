#include <catch2/catch_test_macros.hpp>

#include "../MyArray.h"

class ThrowOnCopy
{
public:
	static inline int copyCount = 0;
	static inline int throwOnCopyCount = 0;

	ThrowOnCopy() = default;

	ThrowOnCopy(const ThrowOnCopy&)
	{
		++copyCount;

		if (copyCount >= throwOnCopyCount)
		{
			throw std::runtime_error("Copy failed");
		}
	}
};

class ThrowOnDefault
{
public:
	static inline int constructCount = 0;
	static inline int throwAfter = 0;

	ThrowOnDefault()
	{
		++constructCount;

		if (constructCount >= throwAfter)
		{
			throw std::runtime_error("Default construction failed");
		}
	}
};

TEST_CASE("Default constructed array is empty")
{
	MyArray<int> array;

	CHECK(array.GetSize() == 0);
	CHECK(array.GetCapacity() == 0);
	CHECK(array.IsEmpty());
}

TEST_CASE("operator[] throws on invalid index")
{
	MyArray<int> array;

	CHECK_THROWS_AS(array[0], std::out_of_range);
}

TEST_CASE("Const operator[] throws on invalid index")
{
	const MyArray<int> array;

	CHECK_THROWS_AS(array[0], std::out_of_range);
}

TEST_CASE("begin equals end for empty array")
{
	MyArray<int> array;

	CHECK(array.begin() == array.end());
	CHECK(array.cbegin() == array.cend());
}

TEST_CASE("Clear keeps array empty")
{
	MyArray<int> array;

	array.Clear();

	CHECK(array.GetSize() == 0);
	CHECK(array.IsEmpty());
}

TEST_CASE("PushBack adds elements")
{
	MyArray<int> array;

	array.PushBack(10);
	array.PushBack(20);
	array.PushBack(30);

	REQUIRE(array.GetSize() == 3);

	CHECK(array[0] == 10);
	CHECK(array[1] == 20);
	CHECK(array[2] == 30);
}

TEST_CASE("Capacity grows exponentially")
{
	MyArray<int> array;

	array.PushBack(1);

	const size_t firstCapacity = array.GetCapacity();

	array.PushBack(2);

	CHECK(array.GetCapacity() >= firstCapacity);
}

TEST_CASE("Range based for works")
{
	MyArray<int> array;

	array.PushBack(1);
	array.PushBack(2);
	array.PushBack(3);

	int sum = 0;

	for (const int value : array)
	{
		sum += value;
	}

	CHECK(sum == 6);
}

TEST_CASE("PushBack supports strings")
{
	MyArray<std::string> array;

	array.PushBack("Hello");
	array.PushBack("World");

	CHECK(array[0] == "Hello");
	CHECK(array[1] == "World");
}

TEST_CASE("PushBack provides strong exception guarantee")
{
	MyArray<ThrowOnCopy> array;

	ThrowOnCopy::copyCount = 0;
	ThrowOnCopy::throwOnCopyCount = 2;

	array.PushBack(ThrowOnCopy{});

	try
	{
		array.PushBack(ThrowOnCopy{});
	}
	catch (...)
	{
	}

	CHECK(array.GetSize() == 1);
}

TEST_CASE("Resize grows array")
{
	MyArray<int> array;

	array.Resize(5);

	REQUIRE(array.GetSize() == 5);

	for (size_t i = 0; i < 5; ++i)
	{
		CHECK(array[i] == 0);
	}
}

TEST_CASE("Resize shrinks array")
{
	MyArray<int> array;

	array.PushBack(10);
	array.PushBack(20);
	array.PushBack(30);

	array.Resize(1);

	REQUIRE(array.GetSize() == 1);
	REQUIRE(array.GetCapacity() == 4);

	CHECK(array[0] == 10);
}

TEST_CASE("Resize to zero")
{
	MyArray<int> array;

	array.PushBack(1);
	array.PushBack(2);

	array.Resize(0);

	CHECK(array.IsEmpty());
}

TEST_CASE("Reverse iterators iterate backwards")
{
	MyArray<int> array;

	array.PushBack(1);
	array.PushBack(2);
	array.PushBack(3);

	std::vector<int> values;

	for (auto it = array.rbegin(); it != array.rend(); ++it)
	{
		values.push_back(*it);
	}

	REQUIRE(values.size() == 3);

	CHECK(values[0] == 3);
	CHECK(values[1] == 2);
	CHECK(values[2] == 1);
}

TEST_CASE("Copy constructor creates deep copy")
{
	MyArray<int> array;

	array.PushBack(1);
	array.PushBack(2);

	MyArray<int> copy(array);

	REQUIRE(copy.GetSize() == 2);

	CHECK(copy[0] == 1);
	CHECK(copy[1] == 2);
}

TEST_CASE("Copy is independent")
{
	MyArray<int> array;

	array.PushBack(10);

	MyArray<int> copy(array);

	copy[0] = 20;

	CHECK(array[0] == 10);
	CHECK(copy[0] == 20);
}

TEST_CASE("Copy assignment works")
{
	MyArray<int> a;
	a.PushBack(1);
	a.PushBack(2);

	MyArray<int> b;
	b.PushBack(100);

	b = a;

	REQUIRE(b.GetSize() == 2);

	CHECK(b[0] == 1);
	CHECK(b[1] == 2);
}

TEST_CASE("Self assignment safe")
{
	MyArray<int> array;

	array.PushBack(42);

	array = array;

	CHECK(array.GetSize() == 1);
	CHECK(array[0] == 42);
}

TEST_CASE("Move constructor transfers ownership")
{
	MyArray<int> array;

	array.PushBack(1);
	array.PushBack(2);

	MyArray<int> moved(std::move(array));

	REQUIRE(moved.GetSize() == 2);

	CHECK(moved[0] == 1);
	CHECK(moved[1] == 2);

	CHECK(array.GetSize() == 0);
}

TEST_CASE("Move assignment transfers ownership")
{
	MyArray<int> a;
	a.PushBack(5);

	MyArray<int> b;
	b = std::move(a);

	CHECK(b.GetSize() == 1);
	CHECK(b[0] == 5);

	CHECK(a.GetSize() == 0);
}

TEST_CASE("Copy assignment strong guarantee")
{
	ThrowOnCopy::copyCount = 0;
	ThrowOnCopy::throwOnCopyCount = 3;

	MyArray<ThrowOnCopy> a;
	a.PushBack(ThrowOnCopy{});

	MyArray<ThrowOnCopy> b;
	b.PushBack(ThrowOnCopy{});

	try
	{
		b = a;
	}
	catch (...)
	{
	}

	CHECK(b.GetSize() == 1);
}

TEST_CASE("Assign int array to double array")
{
	MyArray<int> ints;

	ints.PushBack(1);
	ints.PushBack(2);
	ints.PushBack(3);

	MyArray<double> doubles;

	doubles = ints;

	REQUIRE(doubles.GetSize() == 3);

	CHECK(doubles[0] == 1.0);
	CHECK(doubles[1] == 2.0);
	CHECK(doubles[2] == 3.0);
}

TEST_CASE("Assign double to int array")
{
	MyArray<double> src;

	src.PushBack(1.9);
	src.PushBack(2.1);

	MyArray<int> dst;

	dst = src;

	CHECK(dst[0] == 1);
	CHECK(dst[1] == 2);
}

TEST_CASE("Template assignment same type")
{
	MyArray<int> a;
	a.PushBack(10);

	MyArray<int> b;
	b = a;

	CHECK(b[0] == 10);
}
