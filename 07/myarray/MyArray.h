#ifndef MY_ARRAY_H
#define MY_ARRAY_H

#include <algorithm>
#include <cstddef>
#include <format>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <typename T>
class MyArray
{
public:
	using value_type = T;
	using reference = T&;
	using const_reference = const T&;

	using pointer = T*;
	using const_pointer = const T*;

	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	template <bool IsConst>
	class Iterator
	{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = std::conditional_t<IsConst, const T*, T*>;
		using reference = std::conditional_t<IsConst, const T&, T&>;

		Iterator() noexcept = default;

		explicit Iterator(pointer ptr) noexcept
			: m_ptr(ptr)
		{
		}

		template <bool OtherConst>
		Iterator(const Iterator<OtherConst>& other) noexcept
			: m_ptr(other.m_ptr)
		{
		}

		reference operator*() const noexcept
		{
			return *m_ptr;
		}

		pointer operator->() const noexcept
		{
			return m_ptr;
		}

		reference operator[](difference_type offset) const noexcept
		{
			return *(m_ptr + offset);
		}

		Iterator& operator++() noexcept
		{
			++m_ptr;
			return *this;
		}

		Iterator operator++(int) noexcept
		{
			Iterator temp(*this);
			++(*this);
			return temp;
		}

		Iterator& operator--() noexcept
		{
			--m_ptr;
			return *this;
		}

		Iterator operator--(int) noexcept
		{
			Iterator temp(*this);
			--(*this);
			return temp;
		}

		Iterator& operator+=(difference_type offset) noexcept
		{
			m_ptr += offset;
			return *this;
		}

		Iterator& operator-=(difference_type offset) noexcept
		{
			m_ptr -= offset;
			return *this;
		}

		Iterator operator+(difference_type offset) const noexcept
		{
			Iterator temp(*this);
			temp += offset;
			return temp;
		}

		Iterator operator-(difference_type offset) const noexcept
		{
			Iterator temp(*this);
			temp -= offset;
			return temp;
		}

		difference_type operator-(const Iterator& other) const noexcept
		{
			return m_ptr - other.m_ptr;
		}

		bool operator==(const Iterator& other) const noexcept = default;

		bool operator<(const Iterator& other) const noexcept
		{
			return m_ptr < other.m_ptr;
		}

		bool operator>(const Iterator& other) const noexcept
		{
			return other < *this;
		}

		bool operator<=(const Iterator& other) const noexcept
		{
			return !(other < *this);
		}

		bool operator>=(const Iterator& other) const noexcept
		{
			return !(*this < other);
		}

		friend Iterator operator+(
			difference_type offset,
			const Iterator& iterator) noexcept
		{
			return iterator + offset;
		}

	private:
		pointer m_ptr = nullptr;

		template <bool>
		friend class Iterator;
	};

	using iterator = Iterator<false>;
	using const_iterator = Iterator<true>;

	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	MyArray() noexcept = default;

	MyArray(const MyArray& other)
	{
		CopyFrom(other.begin(), other.m_size);
	}

	MyArray(MyArray&& other) noexcept
		: m_data(other.m_data)
		, m_size(other.m_size)
		, m_capacity(other.m_capacity)
	{
		other.Reset();
	}

	~MyArray() noexcept
	{
		DestroyAndDeallocate(m_data, m_size);
	}

	MyArray& operator=(const MyArray& other)
	{
		if (this != &other)
		{
			MyArray temp(other);
			Swap(temp);
		}

		return *this;
	}

	MyArray& operator=(MyArray&& other) noexcept
	{
		if (this != &other)
		{
			DestroyAndDeallocate(m_data, m_size);

			m_data = other.m_data;
			m_size = other.m_size;
			m_capacity = other.m_capacity;

			other.Reset();
		}

		return *this;
	}

	template <typename U>
	MyArray& operator=(const MyArray<U>& other)
	{
		MyArray temp;

		temp.CopyConvertedFrom(other.begin(), other.GetSize());

		Swap(temp);

		return *this;
	}

	[[nodiscard]]
	size_type GetSize() const noexcept
	{
		return m_size;
	}

	[[nodiscard]]
	size_type GetCapacity() const noexcept
	{
		return m_capacity;
	}

	[[nodiscard]]
	bool IsEmpty() const noexcept
	{
		return m_size == 0;
	}

	void Clear() noexcept
	{
		DestroyRange(m_data, m_size);
		m_size = 0;
	}

	void PushBack(const T& value)
	{
		EmplaceBackInternal(value);
	}

	void PushBack(T&& value)
	{
		EmplaceBackInternal(std::move(value));
	}

	void Resize(size_type newSize)
	{
		if (newSize < m_size)
		{
			DestroyRange(m_data + newSize, m_size - newSize);
			m_size = newSize;
			return;
		}

		if (newSize > m_capacity)
			Reserve(CalculateGrowthCapacity(newSize));

		size_type constructed = m_size;

		try
		{
			for (; constructed < newSize; ++constructed)
				std::construct_at(m_data + constructed);
		}
		catch (...)
		{
			DestroyRange(m_data + m_size, constructed - m_size);
			throw;
		}

		m_size = newSize;
	}

	reference operator[](size_type index)
	{
		CheckIndex(index);
		return m_data[index];
	}

	const_reference operator[](size_type index) const
	{
		CheckIndex(index);
		return m_data[index];
	}

	iterator begin() noexcept
	{
		return iterator(m_data);
	}

	iterator end() noexcept
	{
		return iterator(m_data + m_size);
	}

	const_iterator begin() const noexcept
	{
		return const_iterator(m_data);
	}

	const_iterator end() const noexcept
	{
		return const_iterator(m_data + m_size);
	}

	const_iterator cbegin() const noexcept
	{
		return begin();
	}

	const_iterator cend() const noexcept
	{
		return end();
	}

	reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(end());
	}

	reverse_iterator rend() noexcept
	{
		return reverse_iterator(begin());
	}

	const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(end());
	}

	const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(begin());
	}

	const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(cend());
	}

	const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(cbegin());
	}

	void Swap(MyArray& other) noexcept
	{
		std::swap(m_data, other.m_data);
		std::swap(m_size, other.m_size);
		std::swap(m_capacity, other.m_capacity);
	}

private:
	pointer m_data = nullptr;
	size_type m_size = 0;
	size_type m_capacity = 0;

	template <typename U>
	void EmplaceBackInternal(U&& value)
	{
		if (m_size >= m_capacity)
			Reserve(GrowCapacity(m_capacity));

		std::construct_at(
			m_data + m_size,
			std::forward<U>(value));

		++m_size;
	}

	void Reserve(size_type newCapacity)
	{
		if (newCapacity <= m_capacity)
			return;

		pointer newData = Allocate(newCapacity);

		size_type constructed = 0;

		try
		{
			for (; constructed < m_size; ++constructed)
				std::construct_at(
					newData + constructed,
					std::move_if_noexcept(m_data[constructed]));
		}
		catch (...)
		{
			DestroyRange(newData, constructed);
			::operator delete(newData);
			throw;
		}

		DestroyAndDeallocate(m_data, m_size);

		m_data = newData;
		m_capacity = newCapacity;
	}

	template <typename InputIterator>
	void CopyFrom(InputIterator source, size_type size)
	{
		if (size == 0)
			return;

		pointer data = Allocate(size);

		size_type constructed = 0;

		try
		{
			for (; constructed < size; ++constructed)
				std::construct_at(
					data + constructed,
					*(source + constructed));
		}
		catch (...)
		{
			DestroyRange(data, constructed);
			::operator delete(data);
			throw;
		}

		m_data = data;
		m_size = size;
		m_capacity = size;
	}

	template <typename InputIterator>
	void CopyConvertedFrom(InputIterator source, size_type size)
	{
		if (size == 0)
			return;

		pointer data = Allocate(size);

		size_type constructed = 0;

		try
		{
			for (; constructed < size; ++constructed)
				std::construct_at(
					data + constructed,
					static_cast<T>(*(source + constructed)));
		}
		catch (...)
		{
			DestroyRange(data, constructed);
			::operator delete(data);
			throw;
		}

		m_data = data;
		m_size = size;
		m_capacity = size;
	}

	void CheckIndex(size_type index) const
	{
		if (index >= m_size)
			throw std::out_of_range(std::format("Index {} is out of range", index));
	}

	static pointer Allocate(size_type capacity)
	{
		if (capacity == 0)
			return nullptr;

		return static_cast<pointer>(
			::operator new(sizeof(T) * capacity));
	}

	static void DestroyRange(
		pointer data,
		size_type size) noexcept
	{
		if (data == nullptr)
			return;

		for (size_type i = 0; i < size; ++i)
			std::destroy_at(data + i);
	}

	static void DestroyAndDeallocate(
		pointer data,
		size_type size) noexcept
	{
		DestroyRange(data, size);
		::operator delete(data);
	}

	static size_type GrowCapacity(size_type currentCapacity) noexcept
	{
		return currentCapacity == 0 ? 1 : currentCapacity * 2;
	}

	size_type CalculateGrowthCapacity(size_type requiredCapacity) const noexcept
	{
		size_type capacity = std::max<size_type>(1, m_capacity);

		while (capacity < requiredCapacity)
			capacity *= 2;

		return capacity;
	}

	void Reset() noexcept
	{
		m_data = nullptr;
		m_size = 0;
		m_capacity = 0;
	}
};

#endif // MY_ARRAY_H
