#pragma once

#include <istream>
#include <ostream>

template <typename T>
using Condition = bool(*)(const T&);

template <typename T>
class BaseIterator
{
public:
	virtual void next() = 0;
	virtual T value() const = 0;
	virtual bool are_equal(BaseIterator<T>*) const = 0;
	virtual BaseIterator<T>* clone() const = 0;
	
	virtual ~BaseIterator();
};

//Classes that inherit this abstract class should contain serializable elements =>
//they are considered to be used in the HeteroContainer
template <typename T>
class BaseContainer
{
public:
	virtual bool contains(const T&) const = 0;
	virtual bool contains(Condition<T>) const = 0;
	virtual void filter(Condition<T>) = 0;
	virtual void sort() = 0;
	virtual void push(const T&) = 0;
	virtual T pop() = 0;
	virtual size_t size() const = 0;
	virtual short id() const = 0;
	virtual bool empty() const = 0;
	virtual BaseContainer<T>* clone() const = 0;

	//default value true is going to be used in the regular operations; false is going to be used for serialization purposes
	virtual BaseIterator<T>* begin(bool = true) const = 0;
	virtual BaseIterator<T>* end() const = 0;

	virtual ~BaseContainer();
};

template<typename T>
inline BaseContainer<T>::~BaseContainer()
{}

template<typename T>
inline BaseIterator<T>::~BaseIterator()
{}
