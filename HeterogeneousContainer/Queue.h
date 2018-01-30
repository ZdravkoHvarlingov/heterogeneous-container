#pragma once

#include "BaseContainer.h"
#include "DoublyLinkedList.h"
#include <assert.h>
#include <algorithm>

template <typename T>
class Queue : public BaseContainer<T>
{
public:
	Queue();

	virtual bool contains(const T&) const override;
	virtual bool contains(Condition<T>) const override;
	virtual void filter(Condition<T>);
	virtual void sort();
	virtual void push(const T&) override;
	virtual T pop() override;
	virtual size_t size() const override;
	virtual short id() const override;
	virtual bool empty() const override;
	virtual BaseContainer<T>* clone() const override;

	virtual BaseIterator<T>* begin(bool = true) const;
	virtual BaseIterator<T>* end() const;

	bool operator==(const Queue<T>&) const;

private:
	DoublyLinkedList<T> elements;
};

template<typename T>
inline Queue<T>::Queue()
{}

template<typename T>
inline bool Queue<T>::contains(const T &element) const
{
	return elements.contains(element);
}

template<typename T>
inline bool Queue<T>::contains(Condition<T> pred) const
{
	return elements.contains(pred);
}

template<typename T>
inline void Queue<T>::filter(Condition<T> predicate)
{
	elements.filter(predicate);
}

template<typename T>
inline void Queue<T>::sort()
{
	elements.sort();
}

template<typename T>
inline void Queue<T>::push(const T &element)
{
	elements.push_back(element);
}

template<typename T>
inline T Queue<T>::pop()
{
	return elements.pop();
}

template<typename T>
inline size_t Queue<T>::size() const
{
	return elements.size();
}

template<typename T>
inline short Queue<T>::id() const
{
	return 1;
}

template<typename T>
inline bool Queue<T>::empty() const
{
	return elements.empty();
}

template<typename T>
inline BaseContainer<T>* Queue<T>::clone() const
{
	return new Queue<T>(*this);
}

template<typename T>
inline BaseIterator<T>* Queue<T>::begin(bool useRegular) const
{
	return elements.begin();
}

template<typename T>
inline BaseIterator<T>* Queue<T>::end() const
{
	return elements.end();
}

template<typename T>
inline bool Queue<T>::operator==(const Queue<T> &other) const
{
	elements == other.elements;
}

