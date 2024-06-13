#pragma once

#include <cassert>

template<typename T>
class LinkedList
{
private:
	class LLBlock
	{
	public:
		LLBlock(const T val, LLBlock* prev = nullptr) :
			m_val(val),
			m_prev(prev)
		{
			if (prev != nullptr)
				prev->m_next = this;
		}

		~LLBlock()
		{
			if (m_prev != nullptr)
				m_prev->m_next = m_next;

			if (m_next != nullptr)
				m_next->m_prev = m_prev;
		}

	private:
		T m_val;
		LLBlock* m_prev;
		LLBlock* m_next = nullptr;

		friend class LinkedList;
	};

	class Iterator
	{
	private:
		Iterator(LLBlock* startBlock, bool reversed = false) :
			m_currentBlock(startBlock),
			m_reversed(reversed)
		{
		}

	public:
		Iterator& operator++()
		{
			if (m_currentBlock != nullptr)
				m_currentBlock = m_reversed ? m_currentBlock->m_prev : m_currentBlock->m_next;
			return *this;
		}

		Iterator& operator--()
		{
			if (m_currentBlock != nullptr)
				m_currentBlock = m_reversed ? m_currentBlock->m_next : m_currentBlock->m_prev;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator iterator(m_currentBlock);
			++*this;
			return iterator;
		}

		Iterator operator--(int)
		{
			Iterator iterator(m_currentBlock);
			--*this;
			return iterator;
		}

		T& operator*() const
		{
			assert(m_currentBlock != nullptr);
			return m_currentBlock->m_val;
		}

		bool operator==(const Iterator& other) const
		{ 
			return m_currentBlock == other.m_currentBlock;
		}

		bool operator!=(const Iterator& other) const
		{ 
			return m_currentBlock != other.m_currentBlock;
		}

	private:
		bool m_reversed;
		LLBlock* m_currentBlock;

		friend class LinkedList;
	};

	class IteratorReverseWrapper
	{
	private:
		IteratorReverseWrapper(LLBlock* startBlock) :
			m_startBlock(startBlock)
		{}

	public:
		Iterator begin() const
		{
			return Iterator(m_startBlock, true);
		}

		Iterator end() const
		{
			return Iterator(nullptr);
		}

	private:
		LLBlock* m_startBlock;

		friend class LinkedList;
	};

public:
	LinkedList() = default;

	~LinkedList()
	{
		for (size_t i = 0; i < m_length; i++)
			RemoveLast();
	}

	bool Add(const T val)
	{
		LLBlock* tmp = new LLBlock(val, m_last);
		if (tmp == nullptr)
			return false;

		m_last = tmp;
		if (m_first == nullptr)
			m_first = tmp;

		m_length++;

		return true;
	}

	void Remove(const size_t index, T* ret = nullptr)
	{
		assert(index < m_length);

		LLBlock* tmp = m_first;
		for (size_t i = 0; i < index; i++)
			tmp = tmp->m_next;

		if (ret != nullptr)
			*ret = tmp->m_val;

		if (tmp == m_first)
			m_first = tmp->m_next;

		if (tmp == m_last)
			m_last = tmp->m_prev;

		delete tmp;
		m_length--;
	}

	void RemoveLast(T* ret = nullptr)
	{
		assert(m_length != 0);

		LLBlock* tmp = m_last;
		m_last = m_last->m_prev;

		if (ret != nullptr)
			*ret = tmp->m_val;

		delete tmp;
		m_length--;
	}

	void RemoveFirst(T* ret = nullptr)
	{
		assert(m_length != 0);

		LLBlock* tmp = m_first;
		m_first = m_first->m_next;

		if (ret != nullptr)
			*ret = tmp->m_val;

		delete tmp;
		m_length--;
	}

	void RemoveValue(const T val)
	{
		LLBlock* tmp = m_first;
		while (tmp != nullptr)
		{
			if (tmp->m_val == val)
			{
				if (tmp == m_first)
					m_first = tmp->m_next;

				if (tmp == m_last)
					m_last = tmp->m_prev;

				delete tmp;
				m_length--;
			}

			tmp = tmp->m_next;
		}
	}

	T& Get(size_t index)
	{
		assert(index < m_length);

		LLBlock* tmp = m_first;
		for (size_t i = 0; i < index; i++)
			tmp = tmp->m_next;

		return tmp->m_val;
	}

	bool Find(size_t& retIndex, bool(*filterFunction)(const T& val))
	{
		if (filterFunction == nullptr) return false;

		LLBlock* tmp = m_first;
		size_t index = 0;

		while (tmp != nullptr)
		{
			if (filterFunction(tmp->m_val))
			{
				retIndex = index;
				return true;
			}

			tmp = tmp->m_next;
			index++;
		}

		return false;
	}

	size_t Length() const { return m_length; }

	T& operator[](size_t index)
	{
		assert(index < m_length);

		LLBlock* tmp = m_first;
		for (size_t i = 0; i < index; i++)
			tmp = tmp->m_next;

		return tmp->m_val;
	}

	Iterator begin() const
	{
		return Iterator(m_first);
	}

	Iterator end() const
	{
		return Iterator(nullptr);
	}

	Iterator rbegin() const
	{
		return Iterator(m_last);
	}

	Iterator rend() const
	{
		return Iterator(nullptr);
	}

	IteratorReverseWrapper reverse() const
	{
		return IteratorReverseWrapper(m_last);
	}

private:
	size_t m_length = 0;
	LLBlock* m_first = nullptr;
	LLBlock* m_last = nullptr;
};
