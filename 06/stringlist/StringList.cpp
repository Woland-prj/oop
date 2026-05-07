#include "StringList.hpp"
#include <cstddef>
#include <utility>

StringList::StringList()
{
	m_sentinel = new Node{};
	m_sentinel->next = m_sentinel;
	m_sentinel->prev = m_sentinel;
	m_size = 0;
}

StringList::StringList(const StringList& other)
	: m_sentinel(new Node{})
	, m_size(0)
{
	m_sentinel->next = m_sentinel;
	m_sentinel->prev = m_sentinel;

	for (const auto& val : other)
	{
		try
		{
			// Можно копировать вовременный объект
			PushBack(val);
		}
		catch (const std::exception& ex)
		{
			Clear();
			delete m_sentinel;
			throw ex;
		}
	}
}

// noexcept и new
StringList::StringList(StringList&& other) noexcept
	: m_sentinel(other.m_sentinel)
	, m_size(other.m_size)
{
	other.m_sentinel = new Node{};
	other.m_sentinel->next = other.m_sentinel;
	other.m_sentinel->prev = other.m_sentinel;
	other.m_size = 0;
}

StringList::~StringList() noexcept
{
	Clear();
	delete m_sentinel;
}

StringList& StringList::operator=(const StringList& other)
{
	if (this != &other)
	{
		StringList temp(other);
		Swap(temp);
	}
	return *this;
}

StringList& StringList::operator=(StringList&& other) noexcept
{
	if (this != &other)
	{
		Clear();
		delete m_sentinel;

		m_sentinel = other.m_sentinel;
		m_size = other.m_size;

		// вынести в InitEmpty
		other.m_sentinel = new Node{};
		other.m_sentinel->next = other.m_sentinel;
		other.m_sentinel->prev = other.m_sentinel;
		other.m_size = 0;
	}
	return *this;
}

size_t StringList::Size() const noexcept
{
	return m_size;
}

bool StringList::Empty() const noexcept
{
	return m_size == 0;
}

void StringList::Clear() noexcept
{
	Node* current = m_sentinel->next;
	while (current != m_sentinel)
	{
		Node* toDelete = current;
		current = current->next;
		delete toDelete;
	}
	m_sentinel->next = m_sentinel;
	m_sentinel->prev = m_sentinel;
	m_size = 0;
}

void StringList::Swap(StringList& other) noexcept
{
	std::swap(m_sentinel, other.m_sentinel);
	std::swap(m_size, other.m_size);
}

StringList::iterator StringList::Insert(const_iterator pos, const std::string& value)
{
	Node* position = pos.m_node;
	Node* newNode = nullptr;

	newNode = new Node{ value, nullptr, nullptr };

	Node* prev = position->prev;
	newNode->next = position;
	newNode->prev = prev;
	prev->next = newNode;
	position->prev = newNode;
	++m_size;

	return iterator(newNode);
}

StringList::iterator StringList::Insert(const_iterator pos, std::string&& value)
{
	Node* position = pos.m_node;
	Node* newNode = new Node{ std::move(value), nullptr, nullptr };

	Node* prev = position->prev;
	newNode->next = position;
	newNode->prev = prev;
	prev->next = newNode;
	position->prev = newNode;
	++m_size;

	return iterator(newNode);
}

StringList::iterator StringList::Erase(iterator pos)
{
	Node* node = pos.m_node;
	if (node == m_sentinel)
		return end();

	Node* next = node->next;
	Node* prev = node->prev;

	prev->next = next;
	next->prev = prev;

	delete node;
	--m_size;

	return iterator(next);
}

void StringList::PushBack(const std::string& value)
{
	Insert(end(), value);
}

void StringList::PushBack(std::string&& value)
{
	Insert(end(), std::move(value));
}

void StringList::PushFront(const std::string& value)
{
	Insert(begin(), value);
}

void StringList::PushFront(std::string&& value)
{
	Insert(begin(), std::move(value));
}

StringList::iterator StringList::begin()
{
	return iterator(m_sentinel->next);
}

StringList::iterator StringList::end()
{
	return iterator(m_sentinel);
}

StringList::const_iterator StringList::begin() const
{
	return const_iterator(m_sentinel->next);
}

StringList::const_iterator StringList::end() const
{
	return const_iterator(m_sentinel);
}

StringList::const_iterator StringList::cbegin() const
{
	return const_iterator(m_sentinel->next);
}

StringList::const_iterator StringList::cend() const
{
	return const_iterator(m_sentinel);
}

StringList::reverse_iterator StringList::rbegin()
{
	return reverse_iterator(end());
}

StringList::reverse_iterator StringList::rend()
{
	return reverse_iterator(begin());
}

StringList::const_reverse_iterator StringList::rbegin() const
{
	return const_reverse_iterator(end());
}

StringList::const_reverse_iterator StringList::rend() const
{
	return const_reverse_iterator(begin());
}

StringList::const_reverse_iterator StringList::crbegin() const
{
	return const_reverse_iterator(end());
}

StringList::const_reverse_iterator StringList::crend() const
{
	return const_reverse_iterator(begin());
}
