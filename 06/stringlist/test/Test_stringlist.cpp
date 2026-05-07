#include "StringList.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include <string>
#include <vector>

// тест - Проверять badalloc, все виды итераторов
// тест - проверить O(1), O(N)
TEST(StringListTest, DefaultConstructorCreatesEmptyList)
{
	StringList list;
	EXPECT_EQ(list.Size(), 0);
	EXPECT_TRUE(list.Empty());
	EXPECT_EQ(list.begin(), list.end());
}

TEST(StringListTest, CopyConstructor_EmptyList)
{
	StringList original;
	StringList copy(original);

	EXPECT_EQ(copy.Size(), 0);
	EXPECT_TRUE(copy.Empty());
	EXPECT_EQ(copy.begin(), copy.end());
}

TEST(StringListTest, CopyConstructor_NonEmptyList)
{
	StringList original;
	original.PushBack("first");
	original.PushBack("second");
	original.PushBack("third");

	StringList copy(original);

	EXPECT_EQ(copy.Size(), 3);
	EXPECT_FALSE(copy.Empty());

	auto it = copy.begin();
	EXPECT_EQ(*it++, "first");
	EXPECT_EQ(*it++, "second");
	EXPECT_EQ(*it, "third");
	EXPECT_EQ(std::next(it), copy.end());
}

TEST(StringListTest, CopyConstructor_ForSelfNotWork)
{
	StringList original;
	original.PushBack("data");

	StringList copy(original);
	original.PushBack("new_string");

	EXPECT_EQ(original.Size(), 2);
	EXPECT_EQ(copy.Size(), 1);
	EXPECT_EQ(*copy.begin(), "data");
}

TEST(StringListTest, MoveConstructor_EmptyList)
{
	StringList original;
	StringList moved(std::move(original));

	EXPECT_EQ(moved.Size(), 0);
	EXPECT_TRUE(moved.Empty());
	EXPECT_TRUE(original.Empty());
}

TEST(StringListTest, MoveConstructor_NonEmptyList)
{
	StringList original;
	original.PushBack("m1");
	original.PushBack("m2");

	StringList moved(std::move(original));

	EXPECT_EQ(moved.Size(), 2);
	EXPECT_EQ(*moved.begin(), "m1");
	EXPECT_EQ(*std::next(moved.begin()), "m2");

	EXPECT_TRUE(original.Empty());
	EXPECT_EQ(original.begin(), original.end());
}

TEST(StringListTest, MoveConstructor_AfterMoveOriginalUsable)
{
	StringList original;
	original.PushBack("test");

	StringList moved(std::move(original));

	EXPECT_NO_THROW(original.PushBack("new"));
	EXPECT_EQ(original.Size(), 1);
	EXPECT_EQ(*original.begin(), "new");
}

TEST(StringListTest, CopyAssignment_SelfAssignment)
{
	StringList list;
	list.PushBack("value");

	list = list;

	EXPECT_EQ(list.Size(), 1);
	EXPECT_EQ(*list.begin(), "value");
}

TEST(StringListTest, CopyAssignment_FromEmptyToNonEmpty)
{
	StringList dest;
	dest.PushBack("old1");
	dest.PushBack("old2");

	StringList src;

	dest = src;

	EXPECT_TRUE(dest.Empty());
	EXPECT_EQ(dest.Size(), 0);
	EXPECT_EQ(dest.begin(), dest.end());
}

TEST(StringListTest, CopyAssignment_FromNonEmptyToEmpty)
{
	StringList dest;

	StringList src;
	src.PushBack("copied1");
	src.PushBack("copied2");

	dest = src;

	EXPECT_EQ(dest.Size(), 2);
	auto it = dest.begin();
	EXPECT_EQ(*it++, "copied1");
	EXPECT_EQ(*it, "copied2");
}

TEST(StringListTest, CopyAssignment_FromNonEmptyToNonEmpty)
{
	StringList dest;
	dest.PushBack("old");

	StringList src;
	src.PushBack("new1");
	src.PushBack("new2");

	dest = src;

	EXPECT_EQ(dest.Size(), 2);
	EXPECT_EQ(*dest.begin(), "new1");

	EXPECT_EQ(src.Size(), 2);
}

TEST(StringListTest, MoveAssignment_SelfAssignment)
{
	StringList list;
	list.PushBack("value");

	list = std::move(list);

	EXPECT_NO_THROW(list.Size());
	EXPECT_NO_THROW(list.begin());
}

TEST(StringListTest, MoveAssignment_FromEmptyToNonEmpty)
{
	StringList dest;
	dest.PushBack("old");

	StringList src;

	dest = std::move(src);

	EXPECT_TRUE(dest.Empty());
	EXPECT_TRUE(src.Empty());
}

TEST(StringListTest, MoveAssignment_FromNonEmptyToEmpty)
{
	StringList dest;

	StringList src;
	src.PushBack("moved");

	dest = std::move(src);

	EXPECT_EQ(dest.Size(), 1);
	EXPECT_EQ(*dest.begin(), "moved");
	EXPECT_TRUE(src.Empty());
}

TEST(StringListTest, MoveAssignment_Chain)
{
	StringList a, b, c;
	a.PushBack("A");
	b.PushBack("B");

	c = std::move(b = std::move(a));

	EXPECT_TRUE(a.Empty());
	EXPECT_TRUE(b.Empty());
	EXPECT_EQ(c.Size(), 1);
	EXPECT_EQ(*c.begin(), "A");
}

TEST(StringListTest, Destructor_EmptyList)
{
	{
		StringList list;
	}
	SUCCEED();
}

TEST(StringListTest, Destructor_SingleElement)
{
	{
		StringList list;
		list.PushBack("one");
	}
	SUCCEED();
}

TEST(StringListTest, Destructor_MultipleElements)
{
	{
		StringList list;
		for (int i = 0; i < 100; ++i)
			list.PushBack(std::to_string(i));
	}
	SUCCEED();
}

TEST(StringListTest, Destructor_NoStackOverflow)
{
	{
		StringList list;
		const size_t N = 100000;
		for (size_t i = 0; i < N; ++i)
			list.PushBack(std::to_string(i));
	}
	SUCCEED();
}

TEST(StringListTest, PushBack_ConstRef)
{
	StringList list;
	const std::string val = "test";

	list.PushBack(val);

	EXPECT_EQ(list.Size(), 1);
	EXPECT_EQ(*list.begin(), "test");
	EXPECT_EQ(val, "test");
}

TEST(StringListTest, PushBack_RValue)
{
	StringList list;

	list.PushBack(std::string("temp"));

	EXPECT_EQ(list.Size(), 1);
	EXPECT_EQ(*list.begin(), "temp");
}

TEST(StringListTest, PushFront_ConstRef)
{
	StringList list;
	const std::string val = "first";

	list.PushFront(val);
	list.PushFront("second");

	EXPECT_EQ(list.Size(), 2);
	auto it = list.begin();
	EXPECT_EQ(*it++, "second");
	EXPECT_EQ(*it, "first");
}

TEST(StringListTest, PushFront_RValue)
{
	StringList list;

	list.PushFront(std::string("last"));
	list.PushFront(std::string("first"));

	EXPECT_EQ(*list.begin(), "first");
	EXPECT_EQ(*std::next(list.begin()), "last");
}

TEST(StringListTest, PushBackAndFront_Mixed)
{
	StringList list;

	list.PushBack("back1");
	list.PushFront("front1");
	list.PushBack("back2");
	list.PushFront("front2");

	EXPECT_EQ(list.Size(), 4);

	std::vector<std::string> expected = { "front2", "front1", "back1", "back2" };
	auto it = list.begin();
	for (const auto& exp : expected)
	{
		EXPECT_EQ(*it++, exp);
	}
}

TEST(StringListTest, Size_AfterOperations)
{
	StringList list;

	EXPECT_EQ(list.Size(), 0);

	list.PushBack("a");
	EXPECT_EQ(list.Size(), 1);

	list.PushFront("b");
	EXPECT_EQ(list.Size(), 2);

	list.PushBack("c");
	list.PushBack("d");
	EXPECT_EQ(list.Size(), 4);
}

TEST(StringListTest, Empty_AfterOperations)
{
	StringList list;

	EXPECT_TRUE(list.Empty());

	list.PushBack("x");
	EXPECT_FALSE(list.Empty());
}

TEST(StringListTest, SizeAndEmpty_Const)
{
	const StringList list;

	EXPECT_EQ(list.Size(), 0);
	EXPECT_TRUE(list.Empty());
}

TEST(StringListTest, Clear_EmptyList)
{
	StringList list;

	EXPECT_NO_THROW(list.Clear());
	EXPECT_TRUE(list.Empty());
	EXPECT_EQ(list.Size(), 0);
}

TEST(StringListTest, Clear_SingleElement)
{
	StringList list;
	list.PushBack("one");

	list.Clear();

	EXPECT_TRUE(list.Empty());
	EXPECT_EQ(list.Size(), 0);
	EXPECT_EQ(list.begin(), list.end());
}

TEST(StringListTest, Clear_MultipleElements)
{
	StringList list;
	for (int i = 0; i < 50; ++i)
		list.PushBack(std::to_string(i));

	EXPECT_EQ(list.Size(), 50);

	list.Clear();

	EXPECT_TRUE(list.Empty());
	EXPECT_EQ(list.Size(), 0);

	list.PushBack("after");
	EXPECT_EQ(list.Size(), 1);
	EXPECT_EQ(*list.begin(), "after");
}

TEST(StringListTest, Clear_NoStackOverflow)
{
	StringList list;
	const size_t N = 100000;

	for (size_t i = 0; i < N; ++i)
		list.PushBack(std::to_string(i));

	EXPECT_NO_THROW(list.Clear());
	EXPECT_TRUE(list.Empty());
}

TEST(StringListTest, Clear_Idempotent)
{
	StringList list;
	list.PushBack("test");

	list.Clear();
	list.Clear();
	list.Clear();

	EXPECT_TRUE(list.Empty());
}

TEST(StringListTest, Insert_AtBegin)
{
	StringList list;
	list.PushBack("original");

	auto ret = list.Insert(list.begin(), "inserted");

	EXPECT_EQ(list.Size(), 2);
	EXPECT_EQ(*list.begin(), "inserted");
	EXPECT_EQ(*ret, "inserted");
}

TEST(StringListTest, Insert_AtEnd)
{
	StringList list;
	list.PushBack("first");

	auto ret = list.Insert(list.end(), "last");

	EXPECT_EQ(list.Size(), 2);
	EXPECT_EQ(*std::prev(list.end()), "last");
	EXPECT_EQ(*ret, "last");
}

TEST(StringListTest, Insert_InMiddle)
{
	StringList list;
	list.PushBack("A");
	list.PushBack("C");

	auto it = list.begin();
	++it;

	list.Insert(it, "B");

	EXPECT_EQ(list.Size(), 3);
	auto cur = list.begin();
	EXPECT_EQ(*cur++, "A");
	EXPECT_EQ(*cur++, "B");
	EXPECT_EQ(*cur, "C");
}

TEST(StringListTest, Insert_IntoEmptyList)
{
	StringList list;

	auto ret = list.Insert(list.begin(), "only");

	EXPECT_EQ(list.Size(), 1);
	EXPECT_EQ(*ret, "only");
	EXPECT_EQ(*list.begin(), "only");
	EXPECT_EQ(list.begin(), ret);
}

TEST(StringListTest, Insert_MultipleTimes)
{
	StringList list;

	for (int i = 0; i < 10; ++i)
	{
		list.Insert(list.begin(), std::to_string(i));
	}

	EXPECT_EQ(list.Size(), 10);

	auto it = list.begin();
	for (int i = 9; i >= 0; --i)
	{
		EXPECT_EQ(*it++, std::to_string(i));
	}
}

TEST(StringListTest, Insert_RValue)
{
	StringList list;

	list.Insert(list.end(), std::string("moved"));

	EXPECT_EQ(list.Size(), 1);
	EXPECT_EQ(*list.begin(), "moved");
}

TEST(StringListTest, Erase_SingleElement)
{
	StringList list;
	list.PushBack("to_delete");

	auto ret = list.Erase(list.begin());

	EXPECT_TRUE(list.Empty());
	EXPECT_EQ(ret, list.end());
}

TEST(StringListTest, Erase_FirstElement)
{
	StringList list;
	list.PushBack("first");
	list.PushBack("second");
	list.PushBack("third");

	auto ret = list.Erase(list.begin());

	EXPECT_EQ(list.Size(), 2);
	EXPECT_EQ(*ret, "second");
	EXPECT_EQ(*list.begin(), "second");
}

TEST(StringListTest, Erase_LastElement)
{
	StringList list;
	list.PushBack("first");
	list.PushBack("last");

	auto it = list.begin();
	++it;

	auto ret = list.Erase(it);

	EXPECT_EQ(list.Size(), 1);
	EXPECT_EQ(ret, list.end());
	EXPECT_EQ(*list.begin(), "first");
}

TEST(StringListTest, Erase_MiddleElement)
{
	StringList list;
	list.PushBack("A");
	list.PushBack("B");
	list.PushBack("C");

	auto it = list.begin();
	++it;

	auto ret = list.Erase(it);

	EXPECT_EQ(list.Size(), 2);
	EXPECT_EQ(*ret, "C");

	auto cur = list.begin();
	EXPECT_EQ(*cur++, "A");
	EXPECT_EQ(*cur, "C");
}

TEST(StringListTest, Erase_AllElements)
{
	StringList list;
	for (int i = 0; i < 5; ++i)
		list.PushBack(std::to_string(i));

	while (!list.Empty())
	{
		list.Erase(list.begin());
	}

	EXPECT_TRUE(list.Empty());
	EXPECT_EQ(list.begin(), list.end());
}

TEST(StringListTest, Erase_ReturnsValidIterator)
{
	StringList list;
	list.PushBack("1");
	list.PushBack("2");
	list.PushBack("3");

	for (auto it = list.begin(); it != list.end();)
	{
		if (*it == "2")
			it = list.Erase(it);
		else
			++it;
	}

	EXPECT_EQ(list.Size(), 2);
	auto cur = list.begin();
	EXPECT_EQ(*cur++, "1");
	EXPECT_EQ(*cur, "3");
}

TEST(StringListTest, Iterator_Dereference)
{
	StringList list;
	list.PushBack("value");

	EXPECT_EQ(*list.begin(), "value");
	*list.begin() = "modified";
	EXPECT_EQ(*list.begin(), "modified");
}

TEST(StringListTest, Iterator_PreIncrement)
{
	StringList list;
	list.PushBack("A");
	list.PushBack("B");

	auto it = list.begin();
	++it;

	EXPECT_EQ(*it, "B");
	EXPECT_EQ(std::next(it), list.end());
}

TEST(StringListTest, Iterator_PostIncrement)
{
	StringList list;
	list.PushBack("A");
	list.PushBack("B");

	auto it = list.begin();
	auto old = it++;

	EXPECT_EQ(*old, "A");
	EXPECT_EQ(*it, "B");
}

TEST(StringListTest, Iterator_PreDecrement)
{
	StringList list;
	list.PushBack("A");
	list.PushBack("B");

	auto it = list.end();
	--it;

	EXPECT_EQ(*it, "B");
}

TEST(StringListTest, Iterator_PostDecrement_FromEnd)
{
	StringList list;
	list.PushBack("A");
	list.PushBack("B");

	auto it = list.end();
	auto old = it--;

	EXPECT_EQ(old, list.end());
	EXPECT_EQ(*it, "B");
	EXPECT_NE(it, list.end());
}

TEST(StringListTest, Iterator_Equality)
{
	StringList list;
	list.PushBack("test");

	auto it1 = list.begin();
	auto it2 = list.begin();
	auto end = list.end();

	EXPECT_EQ(it1, it2);
	EXPECT_NE(it1, end);

	++it1;
	EXPECT_NE(it1, it2);
	EXPECT_EQ(it1, end);
}

TEST(StringListTest, Iterator_ArrowOperator)
{
	StringList list;
	list.PushBack("hello");

	EXPECT_EQ(list.begin()->size(), 5);
	EXPECT_EQ(list.begin()->front(), 'h');
}

TEST(StringListTest, ConstIterator_Basic)
{
	const StringList list = []() {
		StringList l;
		l.PushBack("const");
		return l;
	}();

	auto it = list.begin();
	EXPECT_EQ(*it, "const");
}

TEST(StringListTest, RangeBasedFor_NonConst)
{
	StringList list;
	list.PushBack("one");
	list.PushBack("two");
	list.PushBack("three");

	std::vector<std::string> result;
	for (auto& s : list)
	{
		result.push_back(s);
		s += "_mod";
	}

	ASSERT_EQ(result.size(), 3);
	EXPECT_EQ(result[0], "one");

	auto it = list.begin();
	EXPECT_EQ(*it++, "one_mod");
	EXPECT_EQ(*it++, "two_mod");
	EXPECT_EQ(*it, "three_mod");
}

TEST(StringListTest, RangeBasedFor_Const)
{
	const StringList list = []() {
		StringList l;
		l.PushBack("A");
		l.PushBack("B");
		return l;
	}();

	std::vector<std::string> result;
	for (const auto& s : list)
	{
		result.push_back(s);
	}

	ASSERT_EQ(result.size(), 2);
	EXPECT_EQ(result[0], "A");
	EXPECT_EQ(result[1], "B");
}

TEST(StringListTest, ReverseIterator_Basic)
{
	StringList list;
	list.PushBack("first");
	list.PushBack("second");
	list.PushBack("third");

	auto rit = list.rbegin();
	EXPECT_EQ(*rit++, "third");
	EXPECT_EQ(*rit++, "second");
	EXPECT_EQ(*rit, "first");
	EXPECT_EQ(std::next(rit), list.rend());
}

TEST(StringListTest, ReverseIterator_EmptyList)
{
	StringList list;

	EXPECT_EQ(list.rbegin(), list.rend());
}

TEST(StringListTest, ReverseIterator_RangeBasedFor)
{
	StringList list;
	list.PushBack("1");
	list.PushBack("2");
	list.PushBack("3");

	std::vector<std::string> result;
	for (auto it = list.rbegin(); it != list.rend(); ++it)
	{
		result.push_back(*it);
	}

	ASSERT_EQ(result.size(), 3);
	EXPECT_EQ(result[0], "3");
	EXPECT_EQ(result[1], "2");
	EXPECT_EQ(result[2], "1");
}

TEST(StringListTest, ConstReverseIterator)
{
	const StringList list = []() {
		StringList l;
		l.PushBack("X");
		l.PushBack("Y");
		return l;
	}();

	auto rit = list.rbegin();
	EXPECT_EQ(*rit++, "Y");
	EXPECT_EQ(*rit, "X");
}

TEST(StringListTest, Crbegin_Crend)
{
	StringList list;
	list.PushBack("A");
	list.PushBack("B");

	auto rit = list.crbegin();
	EXPECT_EQ(*rit++, "B");
	EXPECT_EQ(*rit, "A");
	EXPECT_EQ(std::next(rit), list.crend());
}

TEST(StringListTest, ReverseIterator_Modification)
{
	StringList list;
	list.PushBack("orig1");
	list.PushBack("orig2");

	*list.rbegin() = "modified";

	EXPECT_EQ(*std::prev(list.end()), "modified");
}

TEST(StringListTest, STL_Find)
{
	StringList list;
	list.PushBack("apple");
	list.PushBack("banana");
	list.PushBack("cherry");

	auto it = std::find(list.begin(), list.end(), "banana");
	EXPECT_NE(it, list.end());
	EXPECT_EQ(*it, "banana");

	auto not_found = std::find(list.begin(), list.end(), "grape");
	EXPECT_EQ(not_found, list.end());
}

TEST(StringListTest, STL_Count)
{
	StringList list;
	list.PushBack("a");
	list.PushBack("b");
	list.PushBack("a");
	list.PushBack("c");

	auto cnt = std::count(list.begin(), list.end(), "a");
	EXPECT_EQ(cnt, 2);
}

TEST(StringListTest, STL_Reverse)
{
	StringList list;
	list.PushBack("1");
	list.PushBack("2");
	list.PushBack("3");

	std::reverse(list.begin(), list.end());

	auto it = list.begin();
	EXPECT_EQ(*it++, "3");
	EXPECT_EQ(*it++, "2");
	EXPECT_EQ(*it, "1");
}

TEST(StringListTest, STL_Transform)
{
	StringList list;
	list.PushBack("hello");
	list.PushBack("world");

	std::transform(list.begin(), list.end(), list.begin(),
		[](const std::string& s) {
			std::string upper = s;
			std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
			return upper;
		});

	auto it = list.begin();
	EXPECT_EQ(*it++, "HELLO");
	EXPECT_EQ(*it, "WORLD");
}

TEST(StringListTest, ExceptionSafety_InsertRollback)
{
	StringList list;
	list.PushBack("original");

	EXPECT_NO_THROW({
		list.Insert(list.end(), std::string("test"));
	});

	EXPECT_EQ(list.Size(), 2);
}

TEST(StringListTest, StrongGuarantee_CopyAssignment)
{
	StringList dest;
	dest.PushBack("keep");

	StringList src;
	src.PushBack("src1");

	EXPECT_NO_THROW(dest = src);

	EXPECT_EQ(dest.Size(), 1);
	EXPECT_EQ(*dest.begin(), "src1");
}

TEST(StringListTest, Noexcept_Destructor)
{
	static_assert(std::is_nothrow_destructible_v<StringList>,
		"Destructor must be noexcept");
}

TEST(StringListTest, Noexcept_MoveOperations)
{
	static_assert(std::is_nothrow_move_constructible_v<StringList>,
		"Move constructor must be noexcept");
	static_assert(std::is_nothrow_move_assignable_v<StringList>,
		"Move assignment must be noexcept");
}

TEST(StringListTest, InsertAtEndThenIterate)
{
	StringList list;

	for (int i = 0; i < 100; ++i)
	{
		list.Insert(list.end(), std::to_string(i));
	}

	EXPECT_EQ(list.Size(), 100);

	int idx = 0;
	for (const auto& s : list)
	{
		EXPECT_EQ(s, std::to_string(idx++));
	}
}

TEST(StringListTest, EraseWhileIterating)
{
	StringList list;
	for (int i = 0; i < 10; ++i)
		list.PushBack(std::to_string(i));

	for (auto it = list.begin(); it != list.end();)
	{
		int val = std::stoi(*it);
		if (val % 2 == 0)
			it = list.Erase(it);
		else
			++it;
	}

	EXPECT_EQ(list.Size(), 5);
	for (const auto& s : list)
	{
		int val = std::stoi(s);
		EXPECT_EQ(val % 2, 1);
	}
}

TEST(StringListTest, Iterator_InvalidationAfterErase)
{
	StringList list;
	list.PushBack("A");
	list.PushBack("B");
	list.PushBack("C");

	auto itA = list.begin();
	auto itB = std::next(itA);
	auto itC = std::next(itB);

	list.Erase(itB);

	EXPECT_EQ(*itA, "A");
	EXPECT_EQ(*itC, "C");
	EXPECT_EQ(std::next(itA), itC);
}

TEST(StringListTest, SwapFunction)
{
	StringList a, b;
	a.PushBack("a1");
	a.PushBack("a2");
	b.PushBack("b1");

	a.Swap(b);

	EXPECT_EQ(a.Size(), 1);
	EXPECT_EQ(*a.begin(), "b1");

	EXPECT_EQ(b.Size(), 2);
	auto it = b.begin();
	EXPECT_EQ(*it++, "a1");
	EXPECT_EQ(*it, "a2");
}

TEST(StringListTest, SwapWithSelf)
{
	StringList list;
	list.PushBack("test");

	EXPECT_NO_THROW(list.Swap(list));

	EXPECT_EQ(list.Size(), 1);
	EXPECT_EQ(*list.begin(), "test");
}

TEST(StringListTest, PushBack_O1_Amortized)
{
	StringList list;
	const size_t N = 10000;

	for (size_t i = 0; i < N; ++i)
	{
		EXPECT_NO_THROW(list.PushBack(std::to_string(i)));
	}

	EXPECT_EQ(list.Size(), N);
}

TEST(StringListTest, InsertAtFront_O1)
{
	StringList list;
	const size_t N = 10000;

	for (size_t i = 0; i < N; ++i)
	{
		EXPECT_NO_THROW(list.PushFront(std::to_string(i)));
	}

	EXPECT_EQ(list.Size(), N);
	EXPECT_EQ(*std::prev(list.end()), "0");
}

TEST(StringListTest, IteratorTypeTraits)
{
	using Iter = StringList::iterator;

	static_assert(std::is_same_v<Iter::iterator_category,
		std::bidirectional_iterator_tag>);
	static_assert(std::is_same_v<Iter::value_type, std::string>);
	static_assert(std::is_same_v<Iter::reference, std::string&>);
	static_assert(std::is_same_v<Iter::pointer, std::string*>);
	static_assert(std::is_same_v<Iter::difference_type, std::ptrdiff_t>);
}

TEST(StringListTest, ConstIteratorTypeTraits)
{
	using CIter = StringList::const_iterator;

	static_assert(std::is_same_v<CIter::reference, const std::string&>);
	static_assert(std::is_same_v<CIter::pointer, const std::string*>);
}

TEST(StringListTest, EmptyList_BeginEndOperations)
{
	StringList list;

	EXPECT_EQ(list.begin(), list.end());
	EXPECT_EQ(list.cbegin(), list.cend());
	EXPECT_EQ(list.rbegin(), list.rend());
	EXPECT_EQ(list.crbegin(), list.crend());

	auto it = list.begin();
	++it;
	EXPECT_EQ(it, list.end());
}

TEST(StringListTest, SingleElementList_AllOperations)
{
	StringList list;
	list.PushBack("only");

	EXPECT_EQ(*list.begin(), "only");
	EXPECT_EQ(*list.rbegin(), "only");
	EXPECT_EQ(std::next(list.begin()), list.end());
	EXPECT_EQ(std::next(list.rbegin()), list.rend());

	list.Erase(list.begin());
	EXPECT_TRUE(list.Empty());
}

TEST(StringListTest, LargeList_ClearAndReuse)
{
	StringList list;

	for (int i = 0; i < 50000; ++i)
		list.PushBack(std::to_string(i));

	EXPECT_EQ(list.Size(), 50000);

	list.Clear();
	EXPECT_TRUE(list.Empty());

	list.PushBack("reused");
	EXPECT_EQ(list.Size(), 1);
	EXPECT_EQ(*list.begin(), "reused");
}

TEST(StringListTest, CopyAfterClear)
{
	StringList list;
	list.PushBack("data");
	list.Clear();

	StringList copy(list);
	EXPECT_TRUE(copy.Empty());
}

TEST(StringListTest, MoveAfterClear)
{
	StringList list;
	list.PushBack("data");
	list.Clear();

	StringList moved(std::move(list));
	EXPECT_TRUE(moved.Empty());
	EXPECT_TRUE(list.Empty());
}
