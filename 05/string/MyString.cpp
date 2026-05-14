#include "MyString.h"

#include <algorithm>
#include <cassert>
#include <istream>
#include <ostream>

char MyString::s_nullTerminator = '\0';
constexpr size_t IO_STACK_BUF_SIZE = 256;

void MyString::CorrectCapacity(size_t newCapacity)
{
	if (newCapacity <= m_capacity)
		return;

	const size_t resizeCapacity = std::max(newCapacity, m_capacity * 2);

	char* newData = new char[resizeCapacity + 1];

	if (m_length > 0 && m_data != &s_nullTerminator)
		std::copy_n(m_data, m_length, newData);

	newData[m_length] = '\0';

	if (m_data != &s_nullTerminator)
		delete[] m_data;

	m_data = newData;
	m_capacity = resizeCapacity;
}

MyString::MyString()
	: m_data(&s_nullTerminator)
	, m_length(0)
	, m_capacity(0)
{
}

MyString::MyString(const char* pString)
	: MyString(pString, pString ? std::strlen(pString) : 0)
{
}

MyString::MyString(const char* pString, size_t length)
	: m_length(length)
	, m_capacity(length)
{
	if (pString == nullptr || length == 0)
	{
		m_data = &s_nullTerminator;
		m_length = 0;
		m_capacity = 0;
	}

	m_data = new char[length + 1];
	std::copy_n(pString, length, m_data);
	m_data[length] = '\0';
}

MyString::MyString(const MyString& other)
	: m_length(other.m_length)
	, m_capacity(other.m_capacity)
{
	if (other.m_data == &s_nullTerminator)
	{
		m_data = &s_nullTerminator;
		m_capacity = 0;
	}

	m_data = new char[m_capacity + 1];
	std::copy_n(other.m_data, m_length, m_data);
	m_data[m_length] = '\0';
}

MyString::MyString(MyString&& other) noexcept
	: m_data(other.m_data)
	, m_length(other.m_length)
	, m_capacity(other.m_capacity)
{
	other.m_data = &s_nullTerminator;
	other.m_capacity = 0;
	other.m_length = 0;
}

MyString::MyString(const std::string& stlString)
	: MyString(stlString.data(), stlString.length())
{
}

MyString::~MyString()
{
	if (m_data != &s_nullTerminator)
		delete[] m_data;
}

size_t MyString::GetLength() const
{
	return m_length;
}

size_t MyString::GetCapacity() const
{
	return m_capacity;
}

const char* MyString::GetStringData() const
{
	return m_data;
}

void MyString::Clear()
{
	m_length = 0;
	if (m_data != &s_nullTerminator)
		m_data[0] = '\0';
}

MyString MyString::SubString(size_t start, size_t length) const
{
	if (start >= m_length)
		return MyString();

	const size_t availableLength = m_length - start;
	const size_t actualLength = std::min(length, availableLength);

	return MyString(m_data + start, actualLength);
}

MyString& MyString::operator=(const MyString& other)
{
	if (this != &other)
	{
		MyString temp(other);
		std::swap(m_data, temp.m_data);
		std::swap(m_length, temp.m_length);
		std::swap(m_capacity, temp.m_capacity);
	}
	return *this;
}
MyString& MyString::operator=(MyString&& other) noexcept
{
	if (this != &other)
	{
		if (m_data != &s_nullTerminator)
			delete[] m_data;

		m_data = other.m_data;
		m_length = other.m_length;
		m_capacity = other.m_capacity;

		other.m_data = &s_nullTerminator;
		other.m_length = 0;
		other.m_capacity = 0;
	}
	return *this;
}

MyString operator+(const MyString& lhs, const MyString& rhs)
{
	MyString result(lhs);
	result += rhs;
	return result;
}

MyString operator+(const std::string& lhs, const MyString& rhs)
{
	return MyString(lhs) + rhs;
}

MyString operator+(const char* lhs, const MyString& rhs)
{
	return MyString(lhs) + rhs;
}

MyString& MyString::operator+=(const MyString& other)
{
	if (other.m_length == 0)
		return *this;

	CorrectCapacity(m_length + other.m_length);

	std::copy_n(other.m_data, other.m_length, m_data + m_length);

	m_length += other.m_length;
	m_data[m_length] = '\0';

	return *this;
}

bool operator==(const MyString& lhs, const MyString& rhs)
{
	if (lhs.m_length != rhs.m_length)
		return false;

	return std::memcmp(lhs.m_data, rhs.m_data, lhs.m_length) == 0;
}

bool operator!=(const MyString& lhs, const MyString& rhs)
{
	return !(lhs == rhs);
}

bool operator<(const MyString& lhs, const MyString& rhs)
{
	const size_t minLen = std::min(lhs.m_length, rhs.m_length);

	const int cmp = std::memcmp(lhs.m_data, rhs.m_data, minLen);

	if (cmp != 0)
		return cmp < 0;

	return lhs.m_length < rhs.m_length;
}

bool operator>(const MyString& lhs, const MyString& rhs)
{
	return rhs < lhs;
}

bool operator<=(const MyString& lhs, const MyString& rhs)
{
	return !(rhs < lhs);
}

bool operator>=(const MyString& lhs, const MyString& rhs)
{
	return !(lhs < rhs);
}

char MyString::operator[](size_t index) const
{
	if (index >= m_length)
		throw std::out_of_range("index > length of string");
	return m_data[index];
}

char& MyString::operator[](size_t index)
{
	if (index >= m_length)
		throw std::out_of_range("index > length of string");
	return m_data[index];
}

std::ostream& operator<<(std::ostream& os, const MyString& str)
{
	if (str.m_length > 0)
		os.write(str.m_data, str.m_length);
	return os;
}

std::istream& operator>>(std::istream& is, MyString& str)
{
	is >> std::ws;
	if (!is || is.eof())
	{
		is.setstate(std::ios::failbit);
		return is;
	}

	char stackBuf[IO_STACK_BUF_SIZE];
	char* buffer = stackBuf;
	size_t capacity = IO_STACK_BUF_SIZE;
	size_t length = 0;

	int ch;
	while ((ch = is.peek()) != EOF && !std::isspace(static_cast<char>(ch), is.getloc()))
	{
		is.get();

		if (length + 1 >= capacity)
		{
			const size_t newCapacity = capacity * 2;
			char* newBuffer = new char[newCapacity];

			std::copy_n(buffer, length, newBuffer);

			if (buffer != stackBuf)
				delete[] buffer;

			buffer = newBuffer;
			capacity = newCapacity;
		}

		buffer[length++] = static_cast<char>(ch);
	}

	if (length > 0)
		str = MyString(buffer, length);
	else
		is.setstate(std::ios::failbit);

	if (buffer != stackBuf)
		delete[] buffer;

	return is;
}

MyString::iterator MyString::begin()
{
	return iterator(m_data);
}

MyString::iterator MyString::end()
{
	return iterator(m_data + m_length);
}

MyString::const_iterator MyString::begin() const
{
	return const_iterator(m_data);
}

MyString::const_iterator MyString::end() const
{
	return const_iterator(m_data + m_length);
}

MyString::const_iterator MyString::cbegin() const
{
	return const_iterator(m_data);
}

MyString::const_iterator MyString::cend() const
{
	return const_iterator(m_data + m_length);
}

MyString::reverse_iterator MyString::rbegin()
{
	return reverse_iterator(end());
}

MyString::reverse_iterator MyString::rend()
{
	return reverse_iterator(begin());
}

MyString::const_reverse_iterator MyString::rbegin() const
{
	return const_reverse_iterator(end());
}

MyString::const_reverse_iterator MyString::rend() const
{
	return const_reverse_iterator(begin());
}

MyString::const_reverse_iterator MyString::crbegin() const
{
	return const_reverse_iterator(cend());
}

MyString::const_reverse_iterator MyString::crend() const
{
	return const_reverse_iterator(cbegin());
}
