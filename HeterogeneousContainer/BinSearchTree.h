#pragma once

#include "BaseContainer.h"
#include "Stack.h"
#include <algorithm>
#include <vector>

template <typename T>
class BinSearchTree : public BaseContainer<T>
{
public:
	template <typename M>
	friend class BSTSortIterator;

	template <typename M>
	friend class BSTPreOrderIterator;

	BinSearchTree();
	BinSearchTree(const BinSearchTree&);
	BinSearchTree<T>& operator= (BinSearchTree<T>);

	virtual bool contains(const T&) const override;
	virtual bool contains(Condition<T>) const override;
	virtual void filter(Condition<T>) override;
	virtual void sort() override; //TO DO

	virtual void push(const T&) override;
	virtual T pop() override;

	virtual size_t size() const override;
	virtual short id() const override;
	virtual bool empty() const override;
	virtual BaseContainer<T>* clone() const override;

	virtual BaseIterator<T>* begin(bool = true) const override;
	virtual BaseIterator<T>* end() const override;

	void remove(const T&);

	~BinSearchTree();

private:
	struct Node
	{
		Node *left;
		T data;
		Node * right;

		Node(const T&, Node* = nullptr, Node* = nullptr);
	};

	bool contains(const T&, Node*) const;
	bool contains(Condition<T>, Node*) const;
	Node* find_min(Node *crr) const;
	void remove(T, Node*&, bool); //we pass T by value because otherwise delete element changes during execution(because it referes tree node)
	void insert(const T&, Node*&);
	void filter(Condition<T>, Node*&);
	void destroy_node(Node *);
	void copy(Node*&, Node*);
	void balance(Node*&, const std::vector<T>&, int, int);

	Node *root;
	size_t count;
};

template <typename T>
class BSTSortIterator : public BaseIterator<T>
{
public:
	BSTSortIterator(typename BinSearchTree<T>::Node*);

	virtual void next() override;
	virtual T value() const override;
	virtual bool are_equal(BaseIterator<T>*) const override;
	virtual BaseIterator<T>* clone() const override;

private:
	void wind();
	Stack<typename BinSearchTree<T>::Node*> nodeStack;
	Stack<bool> shouldProcess;

};

template <typename T>
class BSTPreOrderIterator : public BaseIterator<T>
{
public:
	BSTPreOrderIterator(typename BinSearchTree<T>::Node*);

	virtual void next() override;
	virtual T value() const override;
	virtual bool are_equal(BaseIterator<T>*) const override;
	virtual BaseIterator<T>* clone() const override;

private:
	void wind();
	Stack<std::pair<typename BinSearchTree<T>::Node*, bool>> nodeStack;
};

template<typename T>
inline BinSearchTree<T>::BinSearchTree()
	: root(nullptr), count(0)
{}

template<typename T>
inline BinSearchTree<T>::BinSearchTree(const BinSearchTree &other)
	: BinSearchTree()
{
	copy(root, other.root);
	count = other.count;
}

template<typename T>
inline BinSearchTree<T> & BinSearchTree<T>::operator=(BinSearchTree<T> other)
{
	std::swap(root, other.root);
	std::swap(count, other.count);
}

template<typename T>
inline bool BinSearchTree<T>::contains(const T &element) const
{
	return contains(element, root);
}

template<typename T>
inline bool BinSearchTree<T>::contains(Condition<T> pred) const
{
	return contains(pred, root);
}

template<typename T>
inline void BinSearchTree<T>::filter(Condition<T> pred)
{
	filter(pred, root);
}

template<typename T>
inline void BinSearchTree<T>::sort()
{
	std::vector<int> elements;
	BaseIterator<T> *it = begin();
	BaseIterator<T> *end = this->end();
	while (!it->are_equal(end))
	{
		elements.push_back(it->value());
		it->next();
	}

	delete it; delete end;
	destroy_node(root);
	root = nullptr;
	balance(root, elements, 0, elements.size() - 1);
}

template<typename T>
inline bool BinSearchTree<T>::contains(const T &element, Node *crr) const
{
	if (crr == nullptr) return false;
	if (element == crr->data) return true;

	if (crr->data > element) return contains(element, crr->left);

	return contains(element, crr->right);
}

template<typename T>
inline bool BinSearchTree<T>::contains(Condition<T> pred, Node *crr) const
{
	if (crr == nullptr) return false;
	
	return pred(crr->data) ||
		contains(pred, crr->left) || contains(pred, crr->right);
}

template<typename T>
inline typename BinSearchTree<T>::Node * BinSearchTree<T>::find_min(Node * crr) const
{
	if (crr == nullptr || crr->left == nullptr) return crr;

	return find_min(crr->left);
}

template<typename T>
inline void BinSearchTree<T>::remove(T element, Node *&crr, bool removeEqual)
{
	if (crr == nullptr) return;
	
	if (crr->data == element && crr->left == nullptr)
	{
		Node *save = crr;
		crr = crr->right;
		delete save; --count;

		if (removeEqual) remove(element, crr, true);
		return;
	}

	if (crr->data == element && crr->right == nullptr)
	{
		Node *save = crr;
		crr = crr->left;
		delete save; --count;

		if (removeEqual) remove(element, crr, true);
		return;
	}

	if (crr->data == element)
	{
		Node *rightMin = find_min(crr->right);
		crr->data = rightMin->data;
		remove(rightMin->data, crr->right, false);

		if (removeEqual) remove(element, crr, true); //if there are more than one same elements
		return;
	}

	element < crr->data ? remove(element, crr->left, removeEqual) : remove(element, crr->right, removeEqual);
}

template<typename T>
inline void BinSearchTree<T>::insert(const T &element, Node *&crr)
{
	if (crr == nullptr)
	{
		crr = new Node(element);
		++count;
		return;
	}

	element < crr->data ? insert(element, crr->left) : insert(element, crr->right);
}

template<typename T>
inline void BinSearchTree<T>::filter(Condition<T> pred, Node *&crr)
{
	if (crr == nullptr) return;

	if (pred(crr->data))
	{
		remove(crr->data, crr, true);
		filter(pred, crr);

		return;
	}

	filter(pred, crr->left);
	filter(pred, crr->right);
}

template<typename T>
inline void BinSearchTree<T>::destroy_node(Node *crr)
{
	if (crr == nullptr) return;

	destroy_node(crr->left);
	destroy_node(crr->right);

	delete crr;
}

template<typename T>
inline void BinSearchTree<T>::copy(Node *&thisCrr, Node *otherCrr)
{
	if (otherCrr == nullptr) return;

	thisCrr = new Node(otherCrr->data);
	copy(thisCrr->left, otherCrr->left);
	copy(thisCrr->right, otherCrr->right);
}

template<typename T>
inline void BinSearchTree<T>::balance(Node *&crr, const std::vector<T> &elements, int start, int end)
{
	if (start > end) return;

	int middle = (start + end) / 2;
	crr = new Node(elements[middle]);

	balance(crr->left, elements, start, middle - 1);
	balance(crr->right, elements, middle + 1, end);
}

template<typename T>
inline void BinSearchTree<T>::push(const T &element)
{
	insert(element, root);
}

template<typename T>
inline T BinSearchTree<T>::pop()
{
	T save = root->data;
	remove(root->data, root, false);

	return save;
}

template<typename T>
inline size_t BinSearchTree<T>::size() const
{
	return count;
}

template<typename T>
inline short BinSearchTree<T>::id() const
{
	return 3;
}

template<typename T>
inline bool BinSearchTree<T>::empty() const
{
	return root == nullptr;
}

template<typename T>
inline BaseContainer<T>* BinSearchTree<T>::clone() const
{
	return new BinSearchTree<T>(*this);
}

template<typename T>
inline BaseIterator<T>* BinSearchTree<T>::begin(bool useSortIterator) const
{
	if (useSortIterator)
	{
		return new BSTSortIterator<T>(root);
	}

	return new BSTPreOrderIterator<T>(root);
}

template<typename T>
inline BaseIterator<T>* BinSearchTree<T>::end() const
{
	return new BSTSortIterator<T>(nullptr);
}

template<typename T>
inline BinSearchTree<T>::Node::Node(const T &data, Node *left, Node *right)
	: data(data), left(left), right(right)
{
}

template<typename T>
inline void BinSearchTree<T>::remove(const T &element)
{
	remove(element, root, true);
}

template<typename T>
inline BinSearchTree<T>::~BinSearchTree()
{
	destroy_node(root);
}

template<typename T>
inline BSTSortIterator<T>::BSTSortIterator(typename BinSearchTree<T>::Node *root)
{
	if (root == nullptr) return;
	
	nodeStack.push(root);
	shouldProcess.push(false);
	wind();
}

template<typename T>
inline void BSTSortIterator<T>::next()
{
	nodeStack.pop();
	shouldProcess.pop();
	wind();
}

template<typename T>
inline T BSTSortIterator<T>::value() const
{
	return nodeStack.top()->data;
}

template<typename T>
inline bool BSTSortIterator<T>::are_equal(BaseIterator<T> *other) const
{
	return nodeStack == ((BSTSortIterator<T>*)other)->nodeStack && shouldProcess == ((BSTSortIterator<T>*)other)->shouldProcess;
}

template<typename T>
inline BaseIterator<T>* BSTSortIterator<T>::clone() const
{
	return new BSTSortIterator<T>(*this);
}

template<typename T>
inline void BSTSortIterator<T>::wind()
{
	while (!nodeStack.empty() && shouldProcess.top() != true)
	{
		typename BinSearchTree<T>::Node* crr = nodeStack.pop();
		bool isReady = shouldProcess.pop();

		if (crr->right != nullptr)
		{
			nodeStack.push(crr->right);
			shouldProcess.push(false);
		}
		nodeStack.push(crr);
		shouldProcess.push(true);
			
		if (crr->left != nullptr)
		{
			nodeStack.push(crr->left);
			shouldProcess.push(false);
		}
	}
}

template<typename T>
inline BSTPreOrderIterator<T>::BSTPreOrderIterator(typename BinSearchTree<T>::Node *root)
{
	if (root == nullptr) return;
	
	nodeStack.push(std::make_pair(root, false));
	wind();
}

template<typename T>
inline void BSTPreOrderIterator<T>::next()
{
	nodeStack.pop();
	wind();
}

template<typename T>
inline T BSTPreOrderIterator<T>::value() const
{
	return nodeStack.top().first->data;
}

template<typename T>
inline bool BSTPreOrderIterator<T>::are_equal(BaseIterator<T> *other) const
{
	return nodeStack == ((BSTPreOrderIterator<T>*)other)->nodeStack;
}

template<typename T>
inline BaseIterator<T>* BSTPreOrderIterator<T>::clone() const
{
	return new BSTPreOrderIterator<T>(*this);
}

template<typename T>
inline void BSTPreOrderIterator<T>::wind()
{
	while (!nodeStack.empty() && nodeStack.top().second == false)
	{
		typename BinSearchTree<T>::Node *crr = nodeStack.pop().first;
		
		if (crr->right != nullptr) nodeStack.push(std::make_pair(crr->right, false));
		if (crr->left != nullptr) nodeStack.push(std::make_pair(crr->left, false));
		nodeStack.push(std::make_pair(crr, true));
	}
}
