#pragma once

#include "BaseContainer.h"
#include <assert.h>
#include <algorithm>
#include "DoublyLinkedList.h"

template <typename T>
class Stack : public BaseContainer<T>
{
public:
	Stack();

	virtual bool contains(const T&) const override;
	virtual bool contains(Condition<T>) const;
	virtual void filter(Condition<T>);
	virtual void sort();
	virtual void push(const T&) override;
	virtual T pop() override;
	virtual size_t size() const override;
	virtual short id() const override;
	virtual bool empty() const override;
	virtual BaseContainer<T>* clone() const override;

	virtual BaseIterator<T>* begin(bool = true) const override;
	virtual BaseIterator<T>* end() const override;

	bool operator==(const Stack<T>&) const;

	const T& top() const;
private:

	DoublyLinkedList<T> elements;
};

template<typename T>
inline Stack<T>::Stack()
{}

template<typename T>
inline bool Stack<T>::contains(const T &element) const
{
	return elements.contains(element);
}

template<typename T>
inline bool Stack<T>::contains(Condition<T> pred) const
{
	return elements.contains(pred);
}

template<typename T>
inline void Stack<T>::filter(Condition<T> predicate)
{
	elements.filter(predicate);
}

template<typename T>
inline void Stack<T>::sort()
{
	elements.sort();
}

template<typename T>
inline void Stack<T>::push(const T &element)
{
	elements.push(element);
}

template<typename T>
inline T Stack<T>::pop()
{
	return elements.pop();
}

template<typename T>
inline size_t Stack<T>::size() const
{
	return elements.size();
}

template<typename T>
inline short Stack<T>::id() const
{
	return 0;
}

template<typename T>
inline bool Stack<T>::empty() const
{
	return elements.empty();
}

template<typename T>
inline BaseContainer<T>* Stack<T>::clone() const
{
	return new Stack<T>(*this);
}

template<typename T>
inline BaseIterator<T>* Stack<T>::begin(bool useRegular) const
{
	return elements.begin();
}

template<typename T>
inline BaseIterator<T>* Stack<T>::end() const
{
	return elements.end();
}

template<typename T>
inline bool Stack<T>::operator==(const Stack<T> &other) const
{
	return elements == other.elements;
}

template<typename T>
inline const T & Stack<T>::top() const
{
	assert(!elements.empty());

	return elements.peek_front();
}

