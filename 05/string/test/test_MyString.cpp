#define CATCH_CONFIG_MAIN
#include "MyString.h"
#include "catch2/catch_test_macros.hpp"

#include <algorithm>
#include <map>
#include <sstream>
#include <stdexcept>

#define REQUIRE_STRING_CONTENTS(str, expected_data, expected_len)                          \
	do                                                                                     \
	{                                                                                      \
		REQUIRE((str).GetLength() == (expected_len));                                      \
		REQUIRE(std::memcmp((str).GetStringData(), (expected_data), (expected_len)) == 0); \
		REQUIRE((str).GetStringData()[(expected_len)] == '\0');                            \
	} while (0)

#define REQUIRE_INVARIANTS(str)                                    \
	do                                                             \
	{                                                              \
		REQUIRE((str).GetStringData() != nullptr);                 \
		REQUIRE((str).GetStringData()[(str).GetLength()] == '\0'); \
		REQUIRE((str).GetCapacity() >= (str).GetLength());         \
	} while (0)

TEST_CASE("Default constructor", "[constructor]")
{
	MyString s;

	REQUIRE(s.GetLength() == 0);
	REQUIRE(s.GetCapacity() == 0);
	REQUIRE(s.GetStringData() != nullptr);
	REQUIRE(s.GetStringData()[0] == '\0');
	REQUIRE_INVARIANTS(s);
}

TEST_CASE("Constructor from C-string", "[constructor]")
{
	SECTION("Normal string")
	{
		MyString s("Hello");
		REQUIRE_STRING_CONTENTS(s, "Hello", 5);
		REQUIRE_INVARIANTS(s);
	}

	SECTION("Empty string")
	{
		MyString s("");
		REQUIRE(s.GetLength() == 0);
		REQUIRE(s.GetStringData()[0] == '\0');
		REQUIRE_INVARIANTS(s);
	}

	SECTION("nullptr handling")
	{
		MyString s(nullptr);
		REQUIRE(s.GetLength() == 0);
		REQUIRE(s.GetStringData()[0] == '\0');
		REQUIRE_INVARIANTS(s);
	}

	SECTION("String with special characters")
	{
		MyString s("Hello\tWorld\n");
		REQUIRE_STRING_CONTENTS(s, "Hello\tWorld\n", 12);
	}
}

TEST_CASE("Constructor with explicit length (null char support)", "[constructor][null-char]")
{
	SECTION("String with null in the middle")
	{
		const char raw[] = { 'H', 'i', '\0', '!', 'X' };
		MyString s(raw, 5);

		REQUIRE(s.GetLength() == 5);
		REQUIRE(s.GetStringData()[0] == 'H');
		REQUIRE(s.GetStringData()[1] == 'i');
		REQUIRE(s.GetStringData()[2] == '\0');
		REQUIRE(s.GetStringData()[3] == '!');
		REQUIRE(s.GetStringData()[4] == 'X');
		REQUIRE(s.GetStringData()[5] == '\0');
		REQUIRE_INVARIANTS(s);
	}

	SECTION("String with multiple nulls")
	{
		const char raw[] = { 'a', '\0', 'b', '\0', 'c' };
		MyString s(raw, 5);

		REQUIRE(s.GetLength() == 5);
		REQUIRE(s.GetStringData()[1] == '\0');
		REQUIRE(s.GetStringData()[3] == '\0');
	}

	SECTION("Zero length constructor")
	{
		MyString s("AnyString", 0);
		REQUIRE(s.GetLength() == 0);
		REQUIRE(s.GetStringData()[0] == '\0');
		REQUIRE_INVARIANTS(s);
	}

	SECTION("nullptr with zero length")
	{
		MyString s(nullptr, 0);
		REQUIRE(s.GetLength() == 0);
		REQUIRE_INVARIANTS(s);
	}
}

TEST_CASE("Copy constructor", "[constructor][copy]")
{
	SECTION("Copy normal string")
	{
		MyString original("Hello");
		MyString copy(original);

		REQUIRE(copy == original);
		REQUIRE(copy.GetStringData() != original.GetStringData());
		REQUIRE_INVARIANTS(copy);
		REQUIRE_INVARIANTS(original);
	}

	SECTION("Copy string with null inside")
	{
		const char raw[] = { 'a', '\0', 'b' };
		MyString original(raw, 3);
		MyString copy(original);

		REQUIRE(copy.GetLength() == 3);
		REQUIRE(copy.GetStringData()[1] == '\0');
		REQUIRE(copy.GetStringData() != original.GetStringData());
	}

	SECTION("Copy empty string")
	{
		MyString original;
		MyString copy(original);

		REQUIRE(copy.GetLength() == 0);
		REQUIRE(copy.GetStringData()[0] == '\0');
	}

	SECTION("Independent modification after copy")
	{
		MyString original("Test");
		MyString copy(original);

		copy[0] = 'X';

		REQUIRE(copy[0] == 'X');
		REQUIRE(original[0] == 'T');
	}
}

TEST_CASE("Move constructor", "[constructor][move]")
{
	SECTION("Move normal string")
	{
		MyString original("Hello");
		const char* originalData = original.GetStringData();

		MyString moved(std::move(original));

		REQUIRE(moved.GetLength() == 5);
		REQUIRE(moved.GetStringData() == originalData);
		REQUIRE(moved == "Hello");

		REQUIRE(original.GetLength() == 0);
		REQUIRE(original.GetStringData() != nullptr); // Не nullptr!
		REQUIRE(original.GetStringData()[0] == '\0');
		REQUIRE_INVARIANTS(original);
		REQUIRE_INVARIANTS(moved);
	}

	SECTION("Move string with null inside")
	{
		const char raw[] = { 'X', '\0', 'Y' };
		MyString original(raw, 3);
		const char* originalData = original.GetStringData();

		MyString moved(std::move(original));

		REQUIRE(moved.GetLength() == 3);
		REQUIRE(moved.GetStringData() == originalData);
		REQUIRE(moved.GetStringData()[1] == '\0');
	}

	SECTION("Move empty string")
	{
		MyString original;
		MyString moved(std::move(original));

		REQUIRE(moved.GetLength() == 0);
		REQUIRE(original.GetLength() == 0);
		// Оба должны указывать на статический буфер или быть валидными
		REQUIRE(original.GetStringData()[0] == '\0');
		REQUIRE(moved.GetStringData()[0] == '\0');
	}

	SECTION("Donor is usable after move")
	{
		MyString s1("Original");
		MyString s2(std::move(s1));

		// s1 можно использовать после перемещения!
		s1 = "Reborn";
		REQUIRE(s1 == "Reborn");
		REQUIRE(s2 == "Original"); // s2 сохранил данные
	}

	SECTION("Move in container (vector)")
	{
		std::vector<MyString> vec;
		vec.emplace_back("First");
		vec.emplace_back("Second");

		// При росте vector может перемещать элементы
		vec.emplace_back("Third");
		vec.emplace_back("Fourth");

		REQUIRE(vec[0] == "First");
		REQUIRE(vec[1] == "Second");
		REQUIRE(vec[2] == "Third");
		REQUIRE(vec[3] == "Fourth");
	}
}

TEST_CASE("Constructor from std::string", "[constructor][stl]")
{
	SECTION("Normal std::string")
	{
		std::string stl = "Hello from STL";
		MyString s(stl);

		REQUIRE(s == "Hello from STL");
		REQUIRE(s.GetLength() == stl.length());
	}

	SECTION("Empty std::string")
	{
		std::string stl;
		MyString s(stl);

		REQUIRE(s.GetLength() == 0);
		REQUIRE_INVARIANTS(s);
	}

	SECTION("std::string with null (via data/length)")
	{
		std::string stl("a\0b", 3); // Конструктор std::string с длиной
		MyString s(stl);

		REQUIRE(s.GetLength() == 3);
		REQUIRE(s.GetStringData()[1] == '\0');
	}
}

// ============================================================================
// ТЕСТЫ ДЕСТРУКТОРА (через RAII и утечки памяти)
// ============================================================================

TEST_CASE("Destructor: no memory leaks", "[destructor][memory]")
{
	SECTION("Destroy normal string")
	{
		// Просто создаём и уничтожаем — AddressSanitizer/Valgrind проверит утечки
		{
			MyString s("Temporary");
			REQUIRE(s == "Temporary");
		} // s уничтожается здесь
		// Если есть утечка — тест упадёт при запуске с ASan/Valgrind
	}

	SECTION("Destroy after move")
	{
		{
			MyString s1("Data");
			MyString s2(std::move(s1));
			// Оба уничтожаются: s1 (пустой), s2 (с данными)
		}
	}

	SECTION("Destroy in loop")
	{
		for (int i = 0; i < 100; ++i)
		{
			MyString s("Iteration");
			s += std::to_string(i);
		} // Все временные объекты уничтожаются
	}
}

// ============================================================================
// ТЕСТЫ БАЗОВЫХ МЕТОДОВ
// ============================================================================

TEST_CASE("GetLength", "[getter]")
{
	MyString empty;
	REQUIRE(empty.GetLength() == 0);

	MyString s("Hello");
	REQUIRE(s.GetLength() == 5);

	const char raw[] = { 'a', '\0', 'b' };
	MyString withNull(raw, 3);
	REQUIRE(withNull.GetLength() == 3); // Не 2!
}

TEST_CASE("GetCapacity", "[getter][memory]")
{
	MyString s;
	REQUIRE(s.GetCapacity() == 0); // Пустая строка не выделяет память

	MyString s2("12345");
	REQUIRE(s2.GetCapacity() >= 5); // >= длины

	// Проверяем стратегию удвоения
	MyString growing;
	size_t prevCap = growing.GetCapacity();
	for (int i = 0; i < 100; ++i)
	{
		growing += "x";
		if (growing.GetLength() > prevCap)
		{
			// Вместимость должна была увеличиться
			REQUIRE(growing.GetCapacity() >= growing.GetLength());
			prevCap = growing.GetCapacity();
		}
	}
}

TEST_CASE("GetStringData", "[getter]")
{
	MyString s("Test");
	const char* data = s.GetStringData();

	REQUIRE(data != nullptr);
	REQUIRE(std::strcmp(data, "Test") == 0); // Работает с C-функциями
	REQUIRE(data[4] == '\0'); // Терминатор на месте

	// Строка с '\0' внутри: strcmp не подойдёт, проверяем вручную
	const char raw[] = { 'a', '\0', 'b' };
	MyString withNull(raw, 3);
	const char* nullData = withNull.GetStringData();
	REQUIRE(nullData[0] == 'a');
	REQUIRE(nullData[1] == '\0');
	REQUIRE(nullData[2] == 'b');
	REQUIRE(nullData[3] == '\0'); // Терминатор
}

TEST_CASE("Clear", "[modification]")
{
	SECTION("Clear normal string")
	{
		MyString s("Hello World");
		size_t oldCapacity = s.GetCapacity();

		s.Clear();

		REQUIRE(s.GetLength() == 0);
		REQUIRE(s.GetStringData()[0] == '\0');
		REQUIRE(s.GetCapacity() == oldCapacity); // Буфер не освобождён (оптимизация)
		REQUIRE_INVARIANTS(s);
	}

	SECTION("Clear empty string")
	{
		MyString s;
		s.Clear(); // Не должно крашнуть
		REQUIRE(s.GetLength() == 0);
	}

	SECTION("Reuse after clear")
	{
		MyString s("First");
		s.Clear();
		s += "Second";

		REQUIRE(s == "Second");
		// Буфер переиспользован, новая аллокация не потребовалась
	}

	SECTION("Clear after move")
	{
		MyString s1("Data");
		MyString s2(std::move(s1));

		s1.Clear(); // s1 уже пуст, но это безопасно
		REQUIRE(s1.GetLength() == 0);
	}
}

TEST_CASE("SubString", "[modification]")
{
	SECTION("Extract from middle")
	{
		MyString s("Hello World");
		MyString sub = s.SubString(6, 5); // "World"

		REQUIRE(sub == "World");
		REQUIRE_INVARIANTS(sub);
	}

	SECTION("Extract from beginning")
	{
		MyString s("Hello");
		MyString sub = s.SubString(0, 3); // "Hel"

		REQUIRE(sub == "Hel");
	}

	SECTION("Extract to end (default length)")
	{
		MyString s("Hello");
		MyString sub = s.SubString(2); // "llo" до конца

		REQUIRE(sub == "llo");
	}

	SECTION("Extract with null inside")
	{
		const char raw[] = { 'a', '\0', 'b', 'c', 'd' };
		MyString s(raw, 5);

		MyString sub = s.SubString(1, 3); // "\0bc"

		REQUIRE(sub.GetLength() == 3);
		REQUIRE(sub.GetStringData()[0] == '\0');
		REQUIRE(sub.GetStringData()[1] == 'b');
		REQUIRE(sub.GetStringData()[2] == 'c');
	}

	SECTION("Start beyond length")
	{
		MyString s("Short");
		MyString sub = s.SubString(10, 5); // За границами

		REQUIRE(sub.GetLength() == 0); // Пустая строка
		REQUIRE_INVARIANTS(sub);
	}

	SECTION("Length exceeds available")
	{
		MyString s("Hello");
		MyString sub = s.SubString(3, 100); // Запросили 100, есть только 2

		REQUIRE(sub == "lo"); // Вернулось сколько есть
	}

	SECTION("Empty substring")
	{
		MyString s("Hello");
		MyString sub = s.SubString(2, 0); // Длина 0

		REQUIRE(sub.GetLength() == 0);
	}

	SECTION("Substring of empty string")
	{
		MyString s;
		MyString sub = s.SubString(0, 5);

		REQUIRE(sub.GetLength() == 0);
	}
}

// ============================================================================
// ТЕСТЫ ОПЕРАТОРА ПРИСВАИВАНИЯ
// ============================================================================

TEST_CASE("Copy assignment operator", "[operator=][copy]")
{
	SECTION("Normal assignment")
	{
		MyString s1("Original");
		MyString s2("Temp");

		s2 = s1;

		REQUIRE(s2 == "Original");
		REQUIRE(s2.GetStringData() != s1.GetStringData()); // Глубокое копирование
		REQUIRE_INVARIANTS(s1);
		REQUIRE_INVARIANTS(s2);
	}

	SECTION("Self-assignment")
	{
		MyString s("SomeString");
		const char* originalData = s.GetStringData();

		s = s; // Самоприсваивание

		REQUIRE(s == "SomeString");
		REQUIRE(s.GetStringData() == originalData); // Данные не повреждены
		REQUIRE_INVARIANTS(s);
	}

	SECTION("Assignment to empty")
	{
		MyString empty;
		MyString full("Data");

		empty = full;

		REQUIRE(empty == "Data");
		REQUIRE(full == "Data"); // Источник не изменён
	}

	SECTION("Assignment from empty")
	{
		MyString s("OldData");
		MyString empty;

		s = empty;

		REQUIRE(s.GetLength() == 0);
		REQUIRE(s.GetStringData()[0] == '\0');
	}

	SECTION("Assignment after move")
	{
		MyString s1("First");
		MyString s2("Second");

		MyString s3(std::move(s1)); // s1 теперь пуст
		s1 = s2; // Присваиваем пустому

		REQUIRE(s1 == "Second");
		REQUIRE(s2 == "Second");
		REQUIRE(s3 == "First");
	}

	SECTION("Chained assignment")
	{
		MyString s1("A"), s2("B"), s3("C");

		s1 = s2 = s3;

		REQUIRE(s1 == "C");
		REQUIRE(s2 == "C");
		REQUIRE(s3 == "C");
	}
}

TEST_CASE("Move assignment operator", "[operator=][move]")
{
	SECTION("Normal move assignment")
	{
		MyString s1("Target");
		MyString s2("Source");
		const char* sourceData = s2.GetStringData();

		s1 = std::move(s2);

		REQUIRE(s1.GetStringData() == sourceData); // s1 забрал данные
		REQUIRE(s1 == "Source");

		REQUIRE(s2.GetLength() == 0); // s2 пуст
		REQUIRE(s2.GetStringData()[0] == '\0'); // Но валиден
		REQUIRE_INVARIANTS(s1);
		REQUIRE_INVARIANTS(s2);
	}

	SECTION("Self-move-assignment")
	{
		MyString s("Test");
		const char* originalData = s.GetStringData();

		s = std::move(s);

		REQUIRE(s.GetStringData() != nullptr);
		REQUIRE((s.GetStringData()[0] != '\0' || s.GetLength() == 0));
	}

	SECTION("Move assign to empty")
	{
		MyString empty;
		MyString full("Data");
		const char* fullData = full.GetStringData();

		empty = std::move(full);

		REQUIRE(empty.GetStringData() == fullData);
		REQUIRE(empty == "Data");
		REQUIRE(full.GetLength() == 0);
	}

	SECTION("Move assign frees old resources")
	{
		MyString s1("LongStringThatAllocatesHeapMemory");
		const char* oldData = s1.GetStringData();

		MyString s2("Short");
		const char* s2Data = s2.GetStringData();

		s1 = std::move(s2); // s1 должен освободить старый буфер

		REQUIRE(s1.GetStringData() == s2Data);
		// oldData больше не валиден (удалён), но это проверяется через ASan
	}
}

// ============================================================================
// ТЕСТЫ КОНКАТЕНАЦИИ (+, +=)
// ============================================================================

TEST_CASE("operator+ MyString + MyString", "[operator+][concat]")
{
	MyString a("Hello");
	MyString b("World");

	MyString c = a + b;

	REQUIRE(c == "HelloWorld");
	REQUIRE(a == "Hello"); // Оригинал не изменён
	REQUIRE(b == "World");
	REQUIRE_INVARIANTS(c);
}

TEST_CASE("operator+ std::string + MyString", "[operator+][concat][stl]")
{
	std::string lhs = "C++ ";
	MyString rhs("Rocks");

	MyString result = lhs + rhs;

	REQUIRE(result == "C++ Rocks");
	REQUIRE(rhs == "Rocks"); // rhs не изменён
}

TEST_CASE("operator+ const char* + MyString", "[operator+][concat][cstr]")
{
	const char* lhs = "Hello, ";
	MyString rhs("World!");

	MyString result = lhs + rhs;

	REQUIRE(result == "Hello, World!");
}

TEST_CASE("operator+= MyString", "[operator+=][concat]")
{
	SECTION("Append normal string")
	{
		MyString s("Hello");
		MyString addon(" World");

		s += addon;

		REQUIRE(s == "Hello World");
		REQUIRE(addon == " World"); // addon не изменён
	}

	SECTION("Append empty string")
	{
		MyString s("Test");
		MyString empty;

		s += empty;

		REQUIRE(s == "Test"); // Не изменился
	}

	SECTION("Append to empty string")
	{
		MyString empty;
		MyString data("Data");

		empty += data;

		REQUIRE(empty == "Data");
	}

	SECTION("Append string with null inside")
	{
		const char raw[] = { 'X', '\0', 'Y' };
		MyString s("Start");
		MyString withNull(raw, 3);

		s += withNull;

		REQUIRE(s.GetLength() == 8); // 5 + 3
		REQUIRE(s.GetStringData()[5] == 'X');
		REQUIRE(s.GetStringData()[6] == '\0'); // Нуль внутри сохранён!
		REQUIRE(s.GetStringData()[7] == 'Y');
	}

	SECTION("Multiple appends trigger capacity growth")
	{
		MyString s;
		size_t prevCap = s.GetCapacity();

		for (int i = 0; i < 50; ++i)
		{
			s += "x";
		}

		REQUIRE(s.GetLength() == 50);
		REQUIRE(s.GetCapacity() >= 50);
		// Проверяем, что все символы на месте
		for (size_t i = 0; i < s.GetLength(); ++i)
		{
			REQUIRE(s[i] == 'x');
		}
	}

	SECTION("Self-append (s += s)")
	{
		MyString s("Ab");

		s += s; // "Ab" + "Ab" = "AbAb"

		REQUIRE(s == "AbAb");
		REQUIRE(s.GetLength() == 4);
	}
}

// ============================================================================
// ТЕСТЫ ОПЕРАТОРОВ СРАВНЕНИЯ
// ============================================================================

TEST_CASE("operator== and !=", "[comparison][equality]")
{
	MyString a("abc"), b("abc"), c("abd"), d("ab");

	SECTION("Equal strings")
	{
		REQUIRE(a == b);
		REQUIRE_FALSE(a != b);
	}

	SECTION("Different content")
	{
		REQUIRE(a != c);
		REQUIRE_FALSE(a == c);
	}

	SECTION("Different length")
	{
		REQUIRE(a != d);
		REQUIRE_FALSE(a == d);
	}

	SECTION("Strings with null inside")
	{
		const char raw1[] = { 'a', '\0', 'b' };
		const char raw2[] = { 'a', '\0', 'c' };
		MyString s1(raw1, 3), s2(raw1, 3), s3(raw2, 3);

		REQUIRE(s1 == s2); // Одинаковые
		REQUIRE(s1 != s3); // Разные после '\0'
	}

	SECTION("Empty strings")
	{
		MyString e1, e2;
		REQUIRE(e1 == e2);
	}
}

TEST_CASE("operator< (lexicographical)", "[comparison][ordering]")
{
	SECTION("Basic ordering")
	{
		REQUIRE(MyString("abc") < MyString("abd")); // 'c' < 'd'
		REQUIRE(MyString("abc") < MyString("abcd")); // префикс + короче
		REQUIRE_FALSE(MyString("abd") < MyString("abc"));
	}

	SECTION("Same strings")
	{
		MyString a("test"), b("test");
		REQUIRE_FALSE(a < b);
		REQUIRE_FALSE(b < a);
	}

	SECTION("Empty string ordering")
	{
		MyString empty, nonEmpty("a");
		REQUIRE(empty < nonEmpty);
		REQUIRE_FALSE(nonEmpty < empty);
	}

	SECTION("Null char affects ordering")
	{
		// '\0' имеет код 0, меньше любого печатного символа
		const char raw[] = { 'a', '\0', 'b' };
		MyString withNull(raw, 3);
		MyString without("ab");

		// Сравнение: 'a'=='a', затем '\0' < 'b' → withNull < without
		REQUIRE(withNull < without);
	}

	SECTION("Case sensitivity")
	{
		// ASCII: 'A'=65, 'a'=97 → 'A' < 'a'
		REQUIRE(MyString("Apple") < MyString("apple"));
	}
}

TEST_CASE("operator>, <=, >= (derived)", "[comparison][ordering]")
{
	MyString a("abc"), b("abd"), c("abc");

	SECTION("operator>")
	{
		REQUIRE(b > a); // "abd" > "abc"
		REQUIRE_FALSE(a > b);
		REQUIRE_FALSE(a > c); // равны
	}

	SECTION("operator<=")
	{
		REQUIRE(a <= c); // равны
		REQUIRE(a <= b); // "abc" < "abd"
		REQUIRE_FALSE(b <= a);
	}

	SECTION("operator>=")
	{
		REQUIRE(c >= a); // равны
		REQUIRE(b >= a); // "abd" > "abc"
		REQUIRE_FALSE(a >= b);
	}

	SECTION("Chain of comparisons")
	{
		MyString arr[] = { "apple", "banana", "cherry" };

		REQUIRE(arr[0] < arr[1]);
		REQUIRE(arr[1] < arr[2]);
		REQUIRE(arr[0] < arr[2]);

		REQUIRE(arr[2] > arr[1]);
		REQUIRE(arr[1] > arr[0]);
	}
}

// ============================================================================
// ТЕСТЫ ОПЕРАТОРА ИНДЕКСАЦИИ []
// ============================================================================

TEST_CASE("operator[] const version (read)", "[operator index access][read]")
{
	MyString s("Hello");

	SECTION("Access valid indices")
	{
		REQUIRE(s[0] == 'H');
		REQUIRE(s[1] == 'e');
		REQUIRE(s[4] == 'o');
	}

	SECTION("Access with null inside")
	{
		const char raw[] = { 'a', '\0', 'b' };
		MyString withNull(raw, 3);

		REQUIRE(withNull[0] == 'a');
		REQUIRE(withNull[1] == '\0'); // Чтение нуля — ок
		REQUIRE(withNull[2] == 'b');
	}

	SECTION("Out of bounds throws")
	{
		MyString s("Short");

		REQUIRE_THROWS_AS(s[10], std::out_of_range);
		REQUIRE_THROWS_AS(s[5], std::out_of_range); // length=5, valid: 0..4
	}

	SECTION("Empty string access throws")
	{
		MyString empty;
		REQUIRE_THROWS_AS(empty[0], std::out_of_range);
	}
}

TEST_CASE("operator[] non-const version (write)", "[operator index access][write]")
{
	SECTION("Modify valid index")
	{
		MyString s("Hello");
		s[0] = 'Y';

		REQUIRE(s == "Yello");
		REQUIRE(s[0] == 'Y');
	}

	SECTION("Write null character")
	{
		MyString s("abc");
		s[1] = '\0'; // Записываем нуль внутрь

		REQUIRE(s.GetLength() == 3); // Длина не изменилась!
		REQUIRE(s[1] == '\0');
		REQUIRE(s[2] == 'c'); // Данные после нуля доступны
		// Но при сравнении с "a\0c" через == сработает memcmp, всё корректно
	}

	SECTION("Out of bounds write throws")
	{
		MyString s("Test");

		REQUIRE_THROWS_AS(s[10] = 'X', std::out_of_range);
		REQUIRE(s == "Test"); // Не повреждена при исключении
	}

	SECTION("Modify after copy")
	{
		MyString original("Shared");
		MyString copy = original;

		copy[0] = 'C';

		REQUIRE(copy[0] == 'C');
		REQUIRE(original[0] == 'S'); // Оригинал не затронут (глубокое копирование)
	}
}

// ============================================================================
// ТЕСТЫ ОПЕРАТОРА ВЫВОДА <<
// ============================================================================

TEST_CASE("operator<< output", "[operator<<][iostream]")
{
	SECTION("Output normal string")
	{
		MyString s("Hello");
		std::ostringstream oss;

		oss << s;

		REQUIRE(oss.str() == "Hello");
	}

	SECTION("Output string with null inside")
	{
		const char raw[] = { 'H', 'i', '\0', '!', 'X' };
		MyString s(raw, 5);
		std::ostringstream oss;

		oss << s;

		// write() выводит ровно 5 байт, включая '\0'
		REQUIRE(oss.str().length() == 5);
		REQUIRE(oss.str()[0] == 'H');
		REQUIRE(oss.str()[1] == 'i');
		REQUIRE(oss.str()[2] == '\0'); // Нуль в выводе!
		REQUIRE(oss.str()[3] == '!');
		REQUIRE(oss.str()[4] == 'X');
	}

	SECTION("Output empty string")
	{
		MyString s;
		std::ostringstream oss;

		oss << s;

		REQUIRE(oss.str().empty());
	}

	SECTION("Chained output")
	{
		MyString a("A"), b("B"), c("C");
		std::ostringstream oss;

		oss << a << b << c;

		REQUIRE(oss.str() == "ABC");
	}

	SECTION("Output with formatting")
	{
		MyString s("World");
		std::ostringstream oss;

		oss << "Hello, " << s << "!";

		REQUIRE(oss.str() == "Hello, World!");
	}
}

// ============================================================================
// ТЕСТЫ NULL-СИМВОЛА ВНУТРИ СТРОКИ (критично для задания)
// ============================================================================

TEST_CASE("Null character preservation", "[null-char][critical]")
{
	SECTION("Constructor preserves null")
	{
		const char raw[] = { 'a', '\0', 'b' };
		MyString s(raw, 3);

		REQUIRE(s.GetLength() == 3);
		REQUIRE(s.GetStringData()[1] == '\0');
		REQUIRE(s[1] == '\0');
	}

	SECTION("Copy preserves null")
	{
		const char raw[] = { 'X', '\0', 'Y' };
		MyString original(raw, 3);
		MyString copy(original);

		REQUIRE(copy.GetLength() == 3);
		REQUIRE(copy.GetStringData()[1] == '\0');
		REQUIRE(copy.GetStringData() != original.GetStringData());
	}

	SECTION("Concatenation with null-containing string")
	{
		const char raw[] = { '\0', 'N' };
		MyString prefix("Pre");
		MyString withNull(raw, 2);

		MyString result = prefix + withNull;

		REQUIRE(result.GetLength() == 5); // 3 + 2
		REQUIRE(result.GetStringData()[3] == '\0'); // Нуль на стыке
		REQUIRE(result.GetStringData()[4] == 'N');
	}

	SECTION("Comparison considers null")
	{
		const char raw1[] = { 'a', '\0' };
		const char raw2[] = { 'a', 'b' };
		MyString s1(raw1, 2), s2(raw2, 2);

		// memcmp: 'a'=='a', затем '\0'(0) < 'b'(98)
		REQUIRE(s1 < s2);
		REQUIRE(s1 != s2);
	}

	SECTION("Substring of null-containing string")
	{
		const char raw[] = { 'a', 'b', '\0', 'c', 'd' };
		MyString s(raw, 5);

		MyString sub = s.SubString(2, 2); // "\0c"

		REQUIRE(sub.GetLength() == 2);
		REQUIRE(sub.GetStringData()[0] == '\0');
		REQUIRE(sub.GetStringData()[1] == 'c');
	}

	SECTION("Assignment with null")
	{
		const char raw[] = { '1', '\0', '2' };
		MyString s("Old");
		MyString withNull(raw, 3);

		s = withNull;

		REQUIRE(s.GetLength() == 3);
		REQUIRE(s.GetStringData()[1] == '\0');
	}
}

// ============================================================================
// ТЕСТЫ УПРАВЛЕНИЯ ПАМЯТЬЮ И ЁМКОСТИ
// ============================================================================

TEST_CASE("Capacity growth strategy (doubling)", "[memory][capacity]")
{
	MyString s;

	REQUIRE(s.GetCapacity() == 0);

	// Добавляем символы, отслеживая рост вместимости
	std::vector<size_t> capacityChanges;
	size_t lastCap = 0;

	for (size_t i = 1; i <= 100; ++i)
	{
		s += "x";

		if (s.GetCapacity() != lastCap)
		{
			capacityChanges.push_back(s.GetCapacity());
			lastCap = s.GetCapacity();
		}
	}

	// Проверяем стратегию удвоения: 1, 2, 4, 8, 16, 32, 64, 128...
	REQUIRE(capacityChanges.size() >= 6); // Несколько расширений

	for (size_t i = 1; i < capacityChanges.size(); ++i)
	{
		// Каждая новая вместимость >= предыдущей * 2
		REQUIRE(capacityChanges[i] >= capacityChanges[i - 1] * 2);
	}

	// Финальная вместимость >= длины
	REQUIRE(s.GetCapacity() >= s.GetLength());
	REQUIRE(s.GetLength() == 100);
}

TEST_CASE("Move semantics don't allocate", "[memory][move]")
{
	MyString s("Data");
	const char* originalPtr = s.GetStringData();

	MyString moved(std::move(s));

	REQUIRE(moved.GetStringData() == originalPtr);
}

// ============================================================================
// ТЕСТЫ ИСКЛЮЧЕНИЙ И БЕЗОПАСНОСТИ
// ============================================================================

TEST_CASE("Exception safety of operator[]", "[exception][safety]")
{
	MyString s("Test");

	SECTION("Read out of bounds throws, object unchanged")
	{
		try
		{
			[[maybe_unused]] char c = s[100];
			FAIL("Expected std::out_of_range");
		}
		catch (const std::out_of_range&)
		{
			REQUIRE(s == "Test"); // Объект не повреждён
		}
	}

	SECTION("Write out of bounds throws, object unchanged")
	{
		try
		{
			s[100] = 'X';
			FAIL("Expected std::out_of_range");
		}
		catch (const std::out_of_range&)
		{
			REQUIRE(s == "Test"); // Объект не повреждён
		}
	}
}

TEST_CASE("Strong exception safety of copy assignment", "[exception][safety]")
{
	// Тест сложно проверить без моков, но проверяем базовое поведение:
	// Если копирующий конструктор бросит, *this не должен измениться

	MyString original("Original");
	MyString target("Target");
	const char* targetDataBefore = target.GetStringData();

	// В нормальной ситуации присваивание работает:
	target = original;
	REQUIRE(target == "Original");

	// После присваивания original не должен зависеть от target
	original[0] = 'X';
	REQUIRE(target[0] == 'O'); // target не изменился
}

// ============================================================================
// ТЕСТЫ ИНВАРИАНТОВ КЛАССА
// ============================================================================

TEST_CASE("Class invariants hold after all operations", "[invariant][comprehensive]")
{
	SECTION("After construction")
	{
		MyString s1;
		MyString s2("Test");
		const char raw[] = { 'a', '\0', 'b' };
		MyString s3(raw, 3);

		REQUIRE_INVARIANTS(s1);
		REQUIRE_INVARIANTS(s2);
		REQUIRE_INVARIANTS(s3);
	}

	SECTION("After copy/move")
	{
		MyString original("Data");
		MyString copy(original);
		MyString moved(std::move(original));

		REQUIRE_INVARIANTS(copy);
		REQUIRE_INVARIANTS(moved);
		REQUIRE_INVARIANTS(original); // Донор валиден
	}

	SECTION("After modification")
	{
		MyString s("Hello");

		s += " World";
		REQUIRE_INVARIANTS(s);

		s[0] = 'X';
		REQUIRE_INVARIANTS(s);

		s.Clear();
		REQUIRE_INVARIANTS(s);

		s += "New";
		REQUIRE_INVARIANTS(s);
	}

	SECTION("After substring")
	{
		MyString s("Hello World");
		MyString sub = s.SubString(2, 3);

		REQUIRE_INVARIANTS(s); // Оригинал не повреждён
		REQUIRE_INVARIANTS(sub); // Подстрока валидна
	}

	SECTION("After comparison (no side effects)")
	{
		MyString a("A"), b("B");

		[[maybe_unused]] bool r1 = (a == b);
		[[maybe_unused]] bool r2 = (a < b);
		[[maybe_unused]] bool r3 = (a > b);

		REQUIRE_INVARIANTS(a);
		REQUIRE_INVARIANTS(b);
	}
}

// ============================================================================
// ТЕСТЫ НА ГРАНИЧНЫХ УСЛОВИЯХ
// ============================================================================

TEST_CASE("Edge cases: empty strings", "[edge][empty]")
{
	MyString empty1, empty2;

	SECTION("Empty + Empty")
	{
		MyString result = empty1 + empty2;
		REQUIRE(result.GetLength() == 0);
	}

	SECTION("Empty += Empty")
	{
		empty1 += empty2;
		REQUIRE(empty1.GetLength() == 0);
	}

	SECTION("Empty == Empty")
	{
		REQUIRE(empty1 == empty2);
	}

	SECTION("Empty < NonEmpty")
	{
		MyString nonEmpty("a");
		REQUIRE(empty1 < nonEmpty);
		REQUIRE_FALSE(nonEmpty < empty1);
	}

	SECTION("Substring of empty")
	{
		MyString sub = empty1.SubString(0, 10);
		REQUIRE(sub.GetLength() == 0);
	}
}

TEST_CASE("Edge cases: very long strings", "[edge][long]")
{
	// Создаём строку ~10000 символов
	std::string longStl(10000, 'X');
	MyString s(longStl);

	REQUIRE(s.GetLength() == 10000);
	REQUIRE(s[0] == 'X');
	REQUIRE(s[9999] == 'X');
	REQUIRE_INVARIANTS(s);

	// Операции с длинной строкой
	MyString appended = s + "END";
	REQUIRE(appended.GetLength() == 10003);
	REQUIRE(appended.SubString(10000, 3) == "END");
}

TEST_CASE("Edge cases: single character strings", "[edge][single]")
{
	MyString s("A");

	REQUIRE(s.GetLength() == 1);
	REQUIRE(s[0] == 'A');

	s[0] = 'B';
	REQUIRE(s == "B");

	MyString sub = s.SubString(0, 1);
	REQUIRE(sub == "B");

	MyString emptySub = s.SubString(1, 1); // За границей
	REQUIRE(emptySub.GetLength() == 0);
}

// ============================================================================
// ТЕСТЫ СОВМЕСТИМОСТИ С STL-КОНЦЕПЦИЯМИ
// ============================================================================

TEST_CASE("Swappability (for copy-and-swap)", "[stl][swap]")
{
	MyString a("A"), b("B");

	// std::swap должен работать (используется в operator=)
	std::swap(a, b);

	REQUIRE(a == "B");
	REQUIRE(b == "A");
}

TEST_CASE("Usage in STL containers", "[stl][containers]")
{
	SECTION("Vector of MyString")
	{
		std::vector<MyString> vec;
		vec.push_back("One");
		vec.emplace_back("Two");
		vec.push_back(MyString("Three"));

		REQUIRE(vec.size() == 3);
		REQUIRE(vec[0] == "One");
		REQUIRE(vec[1] == "Two");
		REQUIRE(vec[2] == "Three");

		// Перемещение при росте vector
		vec.reserve(100);
		REQUIRE(vec[0] == "One"); // Данные не повреждены
	}

	SECTION("Map with MyString key")
	{
		std::map<MyString, int> dict;
		dict["apple"] = 1;
		dict["banana"] = 2;

		REQUIRE(dict["apple"] == 1);
		REQUIRE(dict.find("banana") != dict.end());
	}

	SECTION("Sort with comparison operators")
	{
		std::vector<MyString> words = { "cherry", "apple", "banana" };
		std::sort(words.begin(), words.end());

		REQUIRE(words[0] == "apple");
		REQUIRE(words[1] == "banana");
		REQUIRE(words[2] == "cherry");
	}
}

TEST_CASE("Input operator >> basic", "[operator>>]")
{
	std::istringstream iss("Hello World");
	MyString s;

	iss >> s;
	REQUIRE(s == "Hello");
	REQUIRE(iss.good()); // Поток в порядке

	// В потоке осталось " World" — можно прочитать дальше
	iss >> s;
	REQUIRE(s == "World");
}

TEST_CASE("Input operator >> with leading whitespace", "[operator>>]")
{
	std::istringstream iss("\n\t  Trimmed");
	MyString s;

	iss >> s;
	REQUIRE(s == "Trimmed"); // Пробельные символы пропущены
}

TEST_CASE("Input operator >> empty input", "[operator>>]")
{
	std::istringstream iss("   "); // Только пробелы
	MyString s("Original");

	iss >> s;
	REQUIRE(iss.fail()); // Флаг неудачи, т.к. нечего читать
	REQUIRE(s == "Original"); // Строка не изменена
}

TEST_CASE("Input operator >> long string", "[operator>>]")
{
	// Генерируем строку длиннее 256 символов
	std::string longInput(300, 'A');
	std::istringstream iss(longInput);
	MyString s;

	iss >> s;
	REQUIRE(s.GetLength() == 300);
	REQUIRE(s[0] == 'A');
	REQUIRE(s[299] == 'A');
}

TEST_CASE("Input operator >> null chars in input", "[operator>>]")
{
	// Оператор >> читает до пробела, так что '\0' внутри "слова"
	// технически возможен только при бинарном вводе.
	// Но проверяем, что наш MyString корректно хранит данные:
	MyString s("Test");
	std::istringstream iss("A\0B", std::ios::binary); // Бинарный режим

	// Примечание: текстовый operator>> остановится на '\0', т.к.
	// в большинстве локалей '\0' считается управляющим символом.
	// Для чтения строк с '\0' внутри нужен отдельный бинарный метод.
}

TEST_CASE("Iterator begin/end", "[iterator]")
{
	MyString s("Hello");

	auto it = s.begin();
	auto end = s.end();

	REQUIRE(it != end);
	REQUIRE(*it == 'H');

	++it;
	REQUIRE(*it == 'e');

	++it;
	++it;
	++it;

	REQUIRE(*it == 'o');

	++it;
	REQUIRE(it == end);
}

TEST_CASE("Const iterator", "[iterator][const]")
{
	const MyString s("World");

	MyString::const_iterator it = s.begin();

	REQUIRE(*it == 'W');

	++it;
	REQUIRE(*it == 'o');

	static_assert(std::is_same_v<
		decltype(*it),
		const char&>);
}

TEST_CASE("Iterator modification", "[iterator][write]")
{
	MyString s("abc");

	for (auto it = s.begin(); it != s.end(); ++it)
	{
		*it = static_cast<char>(std::toupper(*it));
	}

	REQUIRE(s == "ABC");
}

TEST_CASE("Range-based for support", "[iterator][range-for]")
{
	SECTION("Non-const string")
	{
		MyString s("hello");

		for (char& ch : s)
		{
			ch = static_cast<char>(std::toupper(ch));
		}

		REQUIRE(s == "HELLO");
	}

	SECTION("Const string")
	{
		const MyString s("abc");

		std::string collected;

		for (const char ch : s)
		{
			collected += ch;
		}

		REQUIRE(collected == "abc");
	}
}

TEST_CASE("Iterator arithmetic", "[iterator][arithmetic]")
{
	MyString s("abcdef");

	auto it = s.begin();

	REQUIRE(*(it + 0) == 'a');
	REQUIRE(*(it + 1) == 'b');
	REQUIRE(*(it + 5) == 'f');

	it += 3;
	REQUIRE(*it == 'd');

	it -= 2;
	REQUIRE(*it == 'b');

	auto end = s.end();

	REQUIRE(end - it == 5);
	REQUIRE((it + 5) == end);

	REQUIRE(*(3 + s.begin()) == 'd');
}

TEST_CASE("Iterator indexing operator[]", "[iterator][index]")
{
	MyString s("abcdef");

	auto it = s.begin();

	REQUIRE(it[0] == 'a');
	REQUIRE(it[1] == 'b');
	REQUIRE(it[5] == 'f');

	it[1] = 'X';

	REQUIRE(s == "aXcdef");
}

TEST_CASE("Iterator comparisons", "[iterator][compare]")
{
	MyString s("abc");

	auto b = s.begin();
	auto e = s.end();

	REQUIRE(b < e);
	REQUIRE(e > b);
	REQUIRE(b <= e);
	REQUIRE(e >= b);

	REQUIRE(b == b);
	REQUIRE(b != e);

	++b;
	REQUIRE(*b == 'b');
}

TEST_CASE("Reverse iterator", "[iterator][reverse]")
{
	MyString s("abc");

	std::string reversed;

	for (auto it = s.rbegin(); it != s.rend(); ++it)
	{
		reversed += *it;
	}

	REQUIRE(reversed == "cba");
}

TEST_CASE("Const reverse iterator", "[iterator][reverse][const]")
{
	const MyString s("xyz");

	std::string reversed;

	for (auto it = s.rbegin(); it != s.rend(); ++it)
	{
		reversed += *it;
	}

	REQUIRE(reversed == "zyx");
}

TEST_CASE("STL algorithms compatibility", "[iterator][stl]")
{
	SECTION("std::reverse")
	{
		MyString s("abcdef");

		std::reverse(s.begin(), s.end());

		REQUIRE(s == "fedcba");
	}

	SECTION("std::sort")
	{
		MyString s("dbca");

		std::sort(s.begin(), s.end());

		REQUIRE(s == "abcd");
	}

	SECTION("std::find")
	{
		MyString s("hello");

		auto it = std::find(s.begin(), s.end(), 'l');

		REQUIRE(it != s.end());
		REQUIRE(*it == 'l');
		REQUIRE(it - s.begin() == 2);
	}

	SECTION("std::count")
	{
		MyString s("banana");

		auto count = std::count(s.begin(), s.end(), 'a');

		REQUIRE(count == 3);
	}
}

TEST_CASE("Iterators with null characters", "[iterator][null-char]")
{
	const char raw[] = { 'A', '\0', 'B', '\0', 'C' };

	MyString s(raw, 5);

	auto it = s.begin();

	REQUIRE(*it == 'A');

	++it;
	REQUIRE(*it == '\0');

	++it;
	REQUIRE(*it == 'B');

	++it;
	REQUIRE(*it == '\0');

	++it;
	REQUIRE(*it == 'C');

	++it;
	REQUIRE(it == s.end());
}

TEST_CASE("Iterator distance", "[iterator][distance]")
{
	MyString s("123456789");

	auto b = s.begin();
	auto e = s.end();

	REQUIRE(std::distance(b, e) == 9);

	auto mid = b + 4;

	REQUIRE(std::distance(b, mid) == 4);
	REQUIRE(std::distance(mid, e) == 5);
}

TEST_CASE("Iterator compatibility with std::copy", "[iterator][copy]")
{
	MyString s("CopyTest");

	std::string result;

	std::copy(s.begin(), s.end(), std::back_inserter(result));

	REQUIRE(result == "CopyTest");
}

TEST_CASE("Empty string iterators", "[iterator][empty]")
{
	MyString s;

	REQUIRE(s.begin() == s.end());
	REQUIRE(s.rbegin() == s.rend());

	REQUIRE(std::distance(s.begin(), s.end()) == 0);
}
