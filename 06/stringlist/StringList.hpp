#ifndef STRING_LIST_H
#define STRING_LIST_H

#include <cstddef>
#include <iterator>
#include <string>

class StringList
{
private:
	struct Node
	{
		std::string val;
		Node* prev;
		Node* next;
	};

public:
	template <bool IsConst>
	class Iterator
	{
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = std::string;
		using reference = std::conditional_t<IsConst, const std::string&, std::string&>;
		using pointer = std::conditional_t<IsConst, const std::string*, std::string*>;
		using difference_type = std::ptrdiff_t;

		Iterator(Node* node = nullptr)
			: m_node(node)
		{
		}

		template <bool OtherConst>
		Iterator(const Iterator<OtherConst>& other) noexcept
			requires(!OtherConst && IsConst)
			: m_node(other.m_node)
		{
		}

		reference operator*() const { return m_node->val; }
		pointer operator->() const { return &m_node->val; }

		Iterator& operator++()
		{
			m_node = m_node->next;
			return *this;
		}
		Iterator operator++(int)
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}
		Iterator& operator--()
		{
			m_node = m_node->prev;
			return *this;
		}
		Iterator operator--(int)
		{
			Iterator tmp = *this;
			--(*this);
			return tmp;
		}

		bool operator==(const Iterator& other) const { return m_node == other.m_node; }
		bool operator!=(const Iterator& other) const { return m_node != other.m_node; }

	private:
		Node* m_node;
		friend class StringList;
	};

	using iterator = Iterator<false>;
	using const_iterator = Iterator<true>;

	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	StringList();
	StringList(const StringList& other);
	StringList(StringList&& other) noexcept;
	~StringList() noexcept;

	StringList& operator=(const StringList& other);
	StringList& operator=(StringList&& other) noexcept;

	size_t Size() const noexcept;
	bool Empty() const noexcept;
	void Clear() noexcept;

	iterator Insert(const_iterator pos, const std::string& value);
	iterator Insert(const_iterator pos, std::string&& value);
	iterator Erase(iterator pos);

	void PushBack(const std::string& value);
	void PushBack(std::string&& value);
	void PushFront(const std::string& value);
	void PushFront(std::string&& value);

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

	void Swap(StringList& other) noexcept;

private:
	Node* m_sentinel;
	size_t m_size = 0;
};

#endif // STRING_LIST_H
