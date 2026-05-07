#ifndef MYSTRING_H
#define MYSTRING_H
#include <cstdint>
#include <string>

class MyString
{
public:
	MyString();
	MyString(const char* pString);
	MyString(const char* pString, size_t length);
	MyString(const MyString& other);
	MyString(MyString&& other) noexcept;
	MyString(const std::string& stlString);
	~MyString();

	size_t GetLength() const;
	size_t GetCapacity() const;
	const char* GetStringData() const;
	MyString SubString(size_t start, size_t length = SIZE_MAX) const;

	void Clear();

	MyString& operator=(const MyString& other);
	MyString& operator=(MyString&& other) noexcept;

	friend MyString operator+(const MyString& lhs, const MyString& rhs);
	friend MyString operator+(const std::string& lhs, const MyString& rhs);
	friend MyString operator+(const char* lhs, const MyString& rhs);

	MyString& operator+=(const MyString& other);

	friend bool operator==(const MyString& lhs, const MyString& rhs);
	friend bool operator!=(const MyString& lhs, const MyString& rhs);
	friend bool operator<(const MyString& lhs, const MyString& rhs);
	friend bool operator>(const MyString& lhs, const MyString& rhs);
	friend bool operator<=(const MyString& lhs, const MyString& rhs);
	friend bool operator>=(const MyString& lhs, const MyString& rhs);

	char operator[](size_t index) const;
	char& operator[](size_t index);

	friend std::ostream& operator<<(std::ostream& os, const MyString& str);
	friend std::istream& operator>>(std::istream& is, MyString& str);

private:
	char* m_data;
	size_t m_length;
	size_t m_capacity;

	static char s_nullTerminator;

	void CorrectCapacity(size_t newCapacity);
};

#endif // MYSTRING_H
