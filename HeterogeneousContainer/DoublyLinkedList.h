#pragma once

#include "BaseContainer.h"
#include <assert.h>
#include <algorithm>


template <typename T>
class DoublyLinkedList : public BaseContainer<T>
{
public:
	template <typename M>
	friend class ListIterator;

	DoublyLinkedList();
	DoublyLinkedList(const DoublyLinkedList<T>&);
	DoublyLinkedList<T>& operator=(DoublyLinkedList<T>);

	virtual bool contains(const T&) const override;
	virtual bool contains(Condition<T>) const;
	virtual void filter(Condition<T>);
	virtual void sort();
	virtual void push(const T&) override; //push front
	virtual T pop() override; //pop front
	virtual size_t size() const override;
	virtual short id() const override;
	virtual bool empty() const override;
	virtual BaseContainer<T>* clone() const override;

	virtual BaseIterator<T>* begin(bool = true) const override;
	virtual BaseIterator<T>* end() const override;

	void push_back(const T&);
	T pop_back();
	const T& peek_front() const;
	const T& peek_back() const;
	bool operator==(const DoublyLinkedList<T>&) const;

	virtual ~DoublyLinkedList() override;

private:
	struct Node
	{
		Node *previous;
		T data;
		Node *next;

		Node(Node*, const T&, Node*);
	};

	void DeleteNodeAndChildren(Node*);

	Node *first;
	Node *last;
	size_t count;
};

template <typename T>
class ListIterator : public BaseIterator<T>
{
public:
	ListIterator(typename DoublyLinkedList<T>::Node*);

	virtual void next() override;
	virtual T value() const override;
	virtual bool are_equal(BaseIterator<T>*) const override;
	virtual BaseIterator<T>* clone() const override;

private:
	typename DoublyLinkedList<T>::Node *crr;
};

template<typename T>
inline DoublyLinkedList<T>::Node::Node(Node *previous, const T &data, Node *next)
	: previous(previous), data(data), next(next)
{}

template<typename T>
inline DoublyLinkedList<T>::DoublyLinkedList()
	: first(nullptr), last(nullptr), count(0)
{
}

template<typename T>
inline DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList<T> &other)
	: DoublyLinkedList()
{
	Node *otherCrr = other.last;
	while (otherCrr != nullptr)
	{
		push(otherCrr->data);

		otherCrr = otherCrr->previous;
	}
}

template<typename T>
inline DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(DoublyLinkedList<T> other)
{
	std::swap(first, other.first);
	std::swap(last, other.last);
	std::swap(count, other.count);

	return *this;
}

template<typename T>
inline bool DoublyLinkedList<T>::contains(const T &element) const
{
	Node *crr = first;
	while (crr != nullptr)
	{
		if (crr->data == element) return true;

		crr = crr->next;
	}

	return false;
}

template<typename T>
inline bool DoublyLinkedList<T>::contains(Condition<T> pred) const
{
	Node *crr = first;
	while (crr != nullptr)
	{
		if (pred (crr->data)) return true;

		crr = crr->next;
	}

	return false;
}

template<typename T>
inline void DoublyLinkedList<T>::filter(Condition<T> predicate)
{
	Node *crr = first;
	while (crr != nullptr)
	{
		if (predicate(crr->data))
		{
			if (crr == first)
			{
				crr = crr->next;
				pop();
			}
			else if (crr == last) 
			{
				crr = nullptr;
				pop_back(); 
			}
			else
			{
				Node *next = crr->next;
				
				crr->previous->next = crr->next;
				crr->next->previous = crr->previous;
				delete crr;
				--count;

				crr = next;
			}
		}
		else crr = crr->next;
	}
}

template<typename T>
inline void DoublyLinkedList<T>::sort()
{
	DoublyLinkedList<T> temp = *this;
	while (!empty()) pop();

	while (!temp.empty())
	{
		T top = temp.pop();
		if (empty() || top <= peek_front()) push(top);
		else
		{
			while (!empty() && peek_front() < top) temp.push(pop());
			push(top);
		}
	}
}

template<typename T>
inline void DoublyLinkedList<T>::push(const T &element)
{
	first = new Node(nullptr, element, first);

	if (first->next == nullptr)
	{
		last = first;
	}
	else
	{
		first->next->previous = first;
	}
	++count;
}

template<typename T>
inline T DoublyLinkedList<T>::pop()
{
	assert(first != nullptr);

	T save = first->data;
	Node *oldFirst = first;
	first = first->next;
	delete oldFirst;

	if (first == nullptr)
	{
		last = nullptr;
	}
	else
	{
		first->previous = nullptr;
	}
	--count;

	return save;
}

template<typename T>
inline size_t DoublyLinkedList<T>::size() const
{
	return count;
}

template<typename T>
inline short DoublyLinkedList<T>::id() const
{
	return 2;
}

template<typename T>
inline bool DoublyLinkedList<T>::empty() const
{
	return count == 0;
}

template<typename T>
inline BaseContainer<T>* DoublyLinkedList<T>::clone() const
{
	return new DoublyLinkedList<T>(*this);
}

template<typename T>
inline BaseIterator<T>* DoublyLinkedList<T>::begin(bool useRegular) const
{
	return new ListIterator<T>(first);
}

template<typename T>
inline BaseIterator<T>* DoublyLinkedList<T>::end() const
{
	return new ListIterator<T>(nullptr);
}

template<typename T>
inline void DoublyLinkedList<T>::push_back(const T &data)
{
	last = new Node(last, data, nullptr);

	if (first == nullptr)
	{
		first = last;
	}
	else
	{
		last->previous->next = last;
	}
	++count;
}

template<typename T>
inline T DoublyLinkedList<T>::pop_back()
{
	assert(last != nullptr);

	T save = last->data;
	Node *prvLast = last;
	last = last->previous;
	delete prvLast;

	if (last == nullptr)
	{
		first = nullptr;
	}
	else
	{
		last->next = nullptr;
	}
	--count;

	return save;
}

template<typename T>
inline const T & DoublyLinkedList<T>::peek_front() const
{
	assert(first != nullptr);

	return first->data;
}

template<typename T>
inline const T & DoublyLinkedList<T>::peek_back() const
{
	assert(last != nullptr);

	return last->data;
}

template<typename T>
inline bool DoublyLinkedList<T>::operator==(const DoublyLinkedList<T> &other) const
{
	Node *thisCrr = first;
	Node *otherCrr = other.first;

	while (thisCrr != nullptr && otherCrr != nullptr)
	{
		if (thisCrr->data != otherCrr->data) return false;

		thisCrr = thisCrr->next;
		otherCrr = otherCrr->next;
	}

	return thisCrr == nullptr && otherCrr == nullptr;
}

template<typename T>
inline DoublyLinkedList<T>::~DoublyLinkedList()
{
	DeleteNodeAndChildren(first);
}

template<typename T>
inline void DoublyLinkedList<T>::DeleteNodeAndChildren(Node *crr)
{
	if (crr == nullptr) return;

	DeleteNodeAndChildren(crr->next);
	delete crr;
}

template<typename T>
inline ListIterator<T>::ListIterator(typename DoublyLinkedList<T>::Node *start)
	: crr(start)
{}

template<typename T>
inline void ListIterator<T>::next()
{
	crr = crr->next;
}

template<typename T>
inline T ListIterator<T>::value() const
{
	return crr->data;
}

template<typename T>
inline bool ListIterator<T>::are_equal(BaseIterator<T> *other) const
{
	return ((ListIterator<T>*)other)->crr == crr;
}

template<typename T>
inline BaseIterator<T>* ListIterator<T>::clone() const
{
	return new ListIterator<T>(*this);
}
