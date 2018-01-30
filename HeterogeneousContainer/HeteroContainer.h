#pragma once

#include "DoublyLinkedList.h"
#include "BaseContainer.h"
#include "Stack.h"
#include "Queue.h"
#include "BinSearchTree.h"
#include <vector>

template <typename T>
class HeteroContainer
{
public:
	HeteroContainer();
	HeteroContainer(const HeteroContainer<T>&);
	HeteroContainer<T>& operator=(HeteroContainer<T>);

	enum Type
	{
		STACK = 0,
		QUEUE = 1,
		LINKED_LIST = 2,
		BIN_SEARCH_TREE = 3
	};

	void add_container(Type);
	void add_element(const T&);
	bool contains(const T&) const;
	bool contains(Condition<T> pred) const;
	void filter(Condition<T>);
	void sort();
	size_t elements_size() const;
	size_t containers_size() const;

	class SortIterator;
	SortIterator begin() const; //Container should be sorted in order for the iterator to work properly
	SortIterator end() const;
	SortIterator get_element_it(const T&) const;

	class SpecificIterator;
	SpecificIterator specific_begin(bool inDepth = true) const;
	SpecificIterator specific_end(bool inDepth = true) const;

	template <typename M>
	friend std::ostream& operator<<(std::ostream&, const HeteroContainer<M>&);

	template <typename M>
	friend std::istream& operator>>(std::istream&, HeteroContainer<M>&);

	~HeteroContainer();

private:
	struct Node
	{
		BaseContainer<T> *container;
		Type type;
		Node *next;

		Node(BaseContainer<T>*, Node*, Type);
	};

	BaseContainer<T>* new_container(Type) const;
	Node* get_smallest() const;
	void DeleteNodeAndChildren(Node*);

	Node *first;
	Node *last;
	size_t count;

public:
	class SortIterator
	{
	public:
		SortIterator(Node*, bool = false);
		SortIterator(const SortIterator&);
		SortIterator& operator=(SortIterator);
		
		SortIterator& operator++();
		T operator*() const;
		bool operator!= (SortIterator&) const;

		~SortIterator();

	private:
		void wind();

		Node *first;
		std::vector<BaseIterator<T>*> iterators;
		std::vector<BaseIterator<T>*> ends;
		int crrMin;
		size_t processedElements;
	};

	class SpecificIterator
	{
	public:
		SpecificIterator(Node*, bool = false, bool = true);
		SpecificIterator(const SpecificIterator&);
		SpecificIterator& operator=(SpecificIterator);

		SpecificIterator& operator++();
		T operator*() const;
		bool operator!= (SpecificIterator&) const;

		~SpecificIterator();

	private:
		void wind_depth();
		void wind_breadth();

		Node *first;
		std::vector<BaseIterator<T>*> iterators;
		std::vector<BaseIterator<T>*> ends;
		int ind;
		size_t processedElements;
		bool inDepth;
	};
};

template<typename T>
inline HeteroContainer<T>::HeteroContainer()
	: first(nullptr), last(nullptr), count(0)
{}

template<typename T>
inline HeteroContainer<T>::HeteroContainer(const HeteroContainer<T> &other)
{
	if (other.first == nullptr) return;
	first = new Node(other.first->container->clone(), nullptr, (Type)other.first->container->id());

	Node *otherCrr = other.first;
	Node *thisCrr = first;
	while (otherCrr->next != nullptr)
	{
		thisCrr->next = new Node(otherCrr->next->container->clone(), nullptr, (Type)otherCrr->next->container->id());
		thisCrr = thisCrr->next;
		otherCrr = otherCrr->next;
	}

	count = other.count;
	last = thisCrr;
}

template<typename T>
inline HeteroContainer<T>& HeteroContainer<T>::operator=(HeteroContainer<T> other)
{
	std::swap(first, other.first);
	std::swap(last, other.last);
	std::swap(count, other.count);

	return *this;
}

template<typename T>
inline void HeteroContainer<T>::add_container(Type type)
{
	if (last == nullptr)
	{
		last = new Node(new_container(type), nullptr, type);
		first = last;
	}
	else
	{
		last->next = new Node(new_container(type), nullptr, type);
		last = last->next;
	}

	++count;
}

template<typename T>
inline void HeteroContainer<T>::add_element(const T &element)
{
	assert(count != 0);
	get_smallest()->container->push(element);
}

template<typename T>
inline bool HeteroContainer<T>::contains(const T &element) const
{
	Node *crr = first;
	while (crr != nullptr)
	{
		if (crr->container->contains(element)) return true;

		crr = crr->next;
	}

	return false;
}

template<typename T>
inline bool HeteroContainer<T>::contains(Condition<T> pred) const
{
	Node *crr = first;
	while (crr != nullptr)
	{
		if (crr->container->contains(pred)) return true;

		crr = crr->next;
	}

	return false;
}

template<typename T>
inline void HeteroContainer<T>::filter(Condition<T> predicate)
{
	Node *crr = first;
	while (crr != nullptr)
	{
		crr->container->filter(predicate);

		crr = crr->next;
	}
}

template<typename T>
inline void HeteroContainer<T>::sort()
{
	Node *crr = first;
	while (crr != nullptr)
	{
		crr->container->sort();

		crr = crr->next;
	}
}

template<typename T>
inline size_t HeteroContainer<T>::elements_size() const
{
	size_t result = 0;
	Node *crr = first;
	while (crr != nullptr)
	{
		result += crr->container->size();
	
		crr = crr->next;
	}

	return result;
}

template<typename T>
inline size_t HeteroContainer<T>::containers_size() const
{
	return count;
}

template<typename T>
inline typename HeteroContainer<T>::SortIterator HeteroContainer<T>::begin() const
{
	return SortIterator(first);
}

template<typename T>
inline typename HeteroContainer<T>::SortIterator HeteroContainer<T>::end() const
{
	return SortIterator(first, true);
}

template<typename T>
inline typename HeteroContainer<T>::SortIterator HeteroContainer<T>::get_element_it(const T &element) const
{
	SortIterator it = begin();
	
	while (it != end() && *it != element)
	{
		if (*it > element) return end();
		++it;
	}
	
	return it;
}

template<typename T>
inline typename HeteroContainer<T>::SpecificIterator HeteroContainer<T>::specific_begin(bool inDepth) const
{
	return SpecificIterator(first, false, inDepth);
}

template<typename T>
inline typename HeteroContainer<T>::SpecificIterator HeteroContainer<T>::specific_end(bool inDepth) const
{
	return SpecificIterator(first, true, inDepth);
}

template<typename T>
inline HeteroContainer<T>::~HeteroContainer()
{
	DeleteNodeAndChildren(first);
}

template<typename T>
inline BaseContainer<T> * HeteroContainer<T>::new_container(Type type) const
{
	switch (type)
	{
	case HeteroContainer<T>::STACK: return new Stack<T>;
		break;
	case HeteroContainer<T>::QUEUE: return new Queue<T>;
		break;
	case HeteroContainer<T>::LINKED_LIST: return new DoublyLinkedList<T>;
		break;
	case HeteroContainer<T>::BIN_SEARCH_TREE: return new BinSearchTree<T>;
		break;
	default: return nullptr;
		break;
	}
}

template<typename T>
inline typename HeteroContainer<T>::Node * HeteroContainer<T>::get_smallest() const
{
	Node *crr = first;
	Node *min = first;

	while (crr != nullptr)
	{
		if (crr->container->size() < min->container->size())
		{
			min = crr;
		}

		crr = crr->next;
	}

	return min;
}

template<typename T>
inline void HeteroContainer<T>::DeleteNodeAndChildren(Node *crr)
{
	if (crr == nullptr) return;

	DeleteNodeAndChildren(crr->next);
	delete crr->container;
	delete crr;
}

template<typename T>
inline HeteroContainer<T>::Node::Node(BaseContainer<T> *container, Node *next, Type type)
	: container(container), next(next), type(type)
{}

template<typename T>
inline HeteroContainer<T>::SortIterator::SortIterator(Node *start, bool isEnd)
	: crrMin(-1), processedElements(0), first(start)
{
	Node *crr = start;
	size_t count = 0;
	while (crr != nullptr)
	{
		if (!isEnd) iterators.push_back(crr->container->begin());
		count += crr->container->size();
		if (!isEnd) ends.push_back(crr->container->end());
		
		crr = crr->next;
	}

	if (isEnd) processedElements = count;
	else wind();
}

template<typename T>
inline HeteroContainer<T>::SortIterator::SortIterator(const SortIterator &other)
{
	for (size_t ind = 0; ind < other.iterators.size(); ind++)
	{
		iterators.push_back(other.iterators[ind]->clone());
		ends.push_back(other.ends[ind]->clone());
	}

	processedElements = other.processedElements;
	crrMin = other.crrMin;
	first = other.first;
}

template<typename T>
inline typename HeteroContainer<T>::SortIterator & HeteroContainer<T>::SortIterator::operator=(SortIterator other)
{
	std::swap(crrMin, other.crrMin);
	std::swap(processedElements, other.processedElements);
	std::swap(first, other.first);
	std::swap(iterators, other.iterators);
	std::swap(ends, other.ends);

	return *this;
}

template<typename T>
inline typename HeteroContainer<T>::SortIterator & HeteroContainer<T>::SortIterator::operator++()
{
	iterators[crrMin]->next();
	wind();

	return *this;
}

template<typename T>
inline T HeteroContainer<T>::SortIterator::operator*() const
{
	return iterators[crrMin]->value();
}

template<typename T>
inline bool HeteroContainer<T>::SortIterator::operator!=(SortIterator &other) const
{
	return first != other.first || crrMin != other.crrMin || processedElements != other.processedElements;
}

template<typename T>
inline HeteroContainer<T>::SortIterator::~SortIterator()
{
	for (size_t ind = 0; ind < iterators.size(); ind++)
	{
		delete iterators[ind];
		delete ends[ind];
	}
}

template<typename T>
inline void HeteroContainer<T>::SortIterator::wind()
{
	bool hasMin = false;
	crrMin = -1;

	for (size_t ind = 0; ind < iterators.size(); ind++)
	{
		if (!iterators[ind]->are_equal(ends[ind]) && !hasMin) 
		{
			crrMin = ind;
			hasMin = true;
		}
		else if (!iterators[ind]->are_equal(ends[ind]) && iterators[crrMin]->value() > iterators[ind]->value())
		{
			crrMin = ind;
		}
	}

	if (crrMin != -1) ++processedElements;
}

template<typename M>
inline std::ostream & operator<<(std::ostream &outStr, const HeteroContainer<M> &cont)
{
	outStr << cont.count << "\n";
	typename HeteroContainer<M>::Node *crr = cont.first;
	while (crr != nullptr)
	{
		outStr << crr->type << " " << crr->container->size() << " ";
		BaseIterator<M> *it = crr->container->begin(false); //False because we do NOT use the regular iterator but the serialization one(for the tree)
		BaseIterator<M> *end = crr->container->end();
		while (!it->are_equal(end))
		{
			outStr << it->value() << " ";

			it->next();
		}
		delete it;
		delete end;
		outStr << "\n";

		crr = crr->next;
	}
	outStr << "Where the first number in each line is as follows: 0 - STACK, 1 - QUEUE, 2 - LINKED_LIST, 3 - BIN_SEARCH_TREE." <<
		" The second number in each line is the amount of elements in the current subContainer.";

	return outStr;
}

template<typename M>
inline std::istream & operator>>(std::istream &inStr, HeteroContainer<M> &cont)
{
	size_t amount;
	inStr >> amount;

	HeteroContainer<M> result;
	for (size_t ind = 0; ind < amount; ind++)
	{
		short type;
		inStr >> type;
		result.add_container((typename HeteroContainer<M>::Type)type);
		
		size_t elementsAmount;
		inStr >> elementsAmount;
		for (size_t innerInd = 0; innerInd < elementsAmount; innerInd++)
		{
			M value;
			inStr >> value;
			result.last->container->push(value);
		}
	}

	//move semantics
	//result is temporary, so we serialize inside it and just steal all it's data => 
	//destructor will delete the old container content at the and of the function
	std::swap(cont.first, result.first);
	std::swap(cont.last, result.last);
	std::swap(cont.count, result.count);

	return inStr;
}

template<typename T>
inline HeteroContainer<T>::SpecificIterator::SpecificIterator(Node *start, bool isEnd, bool inDepth)
	: ind(0), first(start), inDepth(inDepth), processedElements(0)
{
	Node *crr = start;
	size_t count = 0;
	size_t indCount = 0;
	while (crr != nullptr)
	{
		if (!isEnd) iterators.push_back(crr->container->begin(false));
		count += crr->container->size();
		++indCount;
		if (!isEnd) ends.push_back(crr->container->end());

		crr = crr->next;
	}

	if (isEnd)
	{
		processedElements = count;
		ind = indCount;
	}
	else if (inDepth) 
	{
		wind_depth();
	}
	else
	{
		ind = -1;
		wind_breadth();
	}
}

template<typename T>
inline HeteroContainer<T>::SpecificIterator::SpecificIterator(const SpecificIterator &other)
{
	for (size_t ind = 0; ind < other.iterators.size(); ind++)
	{
		iterators.push_back(other.iterators[ind]->clone());
		ends.push_back(other.ends[ind]->clone());
	}

	processedElements = other.processedElements;
	ind = other.ind;
	first = other.first;
	inDepth = other.inDepth;
}

template<typename T>
inline typename HeteroContainer<T>::SpecificIterator & HeteroContainer<T>::SpecificIterator::operator=(SpecificIterator other)
{
	std::swap(ind, other.ind);
	std::swap(processedElements, other.processedElements);
	std::swap(first, other.first);
	std::swap(iterators, other.iterators);
	std::swap(ends, other.ends);

	return *this;
}

template<typename T>
inline typename HeteroContainer<T>::SpecificIterator & HeteroContainer<T>::SpecificIterator::operator++()
{
	iterators[ind]->next();
	inDepth ? wind_depth() : wind_breadth();

	return *this;
}

template<typename T>
inline T HeteroContainer<T>::SpecificIterator::operator*() const
{
	return iterators[ind]->value();
}

template<typename T>
inline bool HeteroContainer<T>::SpecificIterator::operator!=(SpecificIterator &other) const
{
	return first != other.first || inDepth != other.inDepth || 
		ind != other.ind || processedElements != other.processedElements;
}

template<typename T>
inline HeteroContainer<T>::SpecificIterator::~SpecificIterator()
{
	for (size_t ind = 0; ind < iterators.size(); ind++)
	{
		delete iterators[ind];
		delete ends[ind];
	}
}

template<typename T>
inline void HeteroContainer<T>::SpecificIterator::wind_depth()
{
	if (iterators[ind]->are_equal(ends[ind])) ++ind;

	if (ind < iterators.size()) ++processedElements;
}

template<typename T>
inline void HeteroContainer<T>::SpecificIterator::wind_breadth()
{
	++ind;
	if (ind >= iterators.size()) ind = 0;
	while (ind < iterators.size() && iterators[ind]->are_equal(ends[ind]))
	{
		++ind;
	}

	if (ind != iterators.size()) ++processedElements;
}
