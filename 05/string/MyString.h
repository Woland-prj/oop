#ifndef MYSTRING_H
#define MYSTRING_H

#include <cassert>
#include <cstdint>
#include <string>

class MyString
{
public:
	template <bool IsConst>
	class Iterator
	{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = char;
		using difference_type = std::ptrdiff_t;
		using pointer = std::conditional_t<IsConst, const char*, char*>;
		using reference = std::conditional_t<IsConst, const char&, char&>;

		Iterator() = default;
		explicit Iterator(pointer ptr)
			: m_ptr(ptr)
		{
		}

		template <bool OtherConst>
		Iterator(const Iterator<OtherConst>& other)
			requires(IsConst || !OtherConst)
			: m_ptr(other.m_ptr)
		{
		}

		reference operator*() const
		{
			assert(m_ptr != nullptr);
			return *m_ptr;
		}

		pointer operator->() const
		{
			return m_ptr;
		}

		reference operator[](difference_type index) const
		{
			assert(m_ptr != nullptr);
			return m_ptr[index];
		}

		Iterator& operator++()
		{
			++m_ptr;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator temp(*this);
			++(*this);
			return temp;
		}

		Iterator& operator--()
		{
			--m_ptr;
			return *this;
		}

		Iterator operator--(int)
		{
			Iterator temp(*this);
			--(*this);
			return temp;
		}

		Iterator& operator+=(difference_type n)
		{
			m_ptr += n;
			return *this;
		}

		Iterator& operator-=(difference_type n)
		{
			m_ptr -= n;
			return *this;
		}

		Iterator operator+(difference_type n) const
		{
			return Iterator(m_ptr + n);
		}

		Iterator operator-(difference_type n) const
		{
			return Iterator(m_ptr - n);
		}

		difference_type operator-(const Iterator& other) const
		{
			return m_ptr - other.m_ptr;
		}

		friend Iterator operator+(
			difference_type n,
			const Iterator& it)
		{
			return it + n;
		}

		bool operator==(const Iterator& other) const = default;
		std::strong_ordering operator<=>(const Iterator& other) const = default;

	private:
		pointer m_ptr = nullptr;
	};

	using iterator = Iterator<false>;
	using const_iterator = Iterator<true>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

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

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;

	reverse_iterator rbegin();
	reverse_iterator rend();
	const_reverse_iterator rbegin() const;
	const_reverse_iterator rend() const;
	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;

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
