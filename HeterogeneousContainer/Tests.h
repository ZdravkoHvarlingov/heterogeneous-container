#pragma once

#include "Stack.h"
#include "Queue.h"
#include "DoublyLinkedList.h"
#include "BinSearchTree.h"
#include "HeteroContainer.h"

void TestStack()
{
	int numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	
	Stack<int> st;
	for (int number : numbers) st.push(number);

	BaseIterator<int> *it = st.begin();
	BaseIterator<int> *end = st.end();
	int count = 8;
	while (!it->are_equal(end))
	{
		assert(numbers[count] == it->value());
		
		it->next();
		--count;
	}

	st.sort();
	count = 0;
	delete it;
	it = st.begin();
	while (!it->are_equal(end))
	{
		assert(numbers[count] == it->value());

		it->next();
		++count;
	}
	delete it;
	delete end;

	assert(st.contains(3));
	assert(st.contains([](const int &number) { return number % 2 == 0; }));
	assert(!st.contains([](const int &number) { return number == 11; }));
	assert(!st.empty());
	assert(st.size() == 9);
	
	while (!st.empty()) st.pop();
	assert(st.size() == 0);
}

void TestQueue()
{
	int numbers[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1 };

	Queue<int> que;
	for (int number : numbers) que.push(number);

	BaseIterator<int> *it = que.begin();
	BaseIterator<int> *end = que.end();
	int count = 0;
	while (!it->are_equal(end))
	{
		assert(numbers[count] == it->value());

		it->next();
		++count;
	}

	que.sort();
	count = 8;
	delete it;
	it = que.begin();
	while (!it->are_equal(end))
	{
		assert(numbers[count] == it->value());

		it->next();
		--count;
	}
	delete it;
	delete end;

	assert(que.contains(3));
	assert(que.contains([](const int &number) { return number % 2 == 0; }));
	assert(!que.contains([](const int &number) { return number == 11; }));
	assert(!que.empty());
	assert(que.size() == 9);

	while (!que.empty()) que.pop();
	assert(que.size() == 0);
}

void TestBinSearchTree()
{
	BinSearchTree<int> tree;
	int numbers[] = { 9, -8, 7, -6, 5, -4, 3, -2, 1 };

	for (int number : numbers) tree.push(number);
	tree.sort();

	BaseIterator<int> *it = tree.begin();
	BaseIterator<int> *end = tree.end();
	bool hasPrev = false;
	int previous = 0;
	while (!it->are_equal(end))
	{
		if (hasPrev) assert(previous <= it->value());
		else hasPrev = true;

		previous = it->value();
		it->next();
	}

	delete it; delete end;

	assert(tree.size() == 9);
	assert(!tree.contains(-9));
	assert(tree.contains(9));

	tree.remove(9);
	assert(!tree.contains(9));
	assert(tree.size() == 8);
	assert(!tree.empty());

	tree.filter([](const int &number) { return number % 2 == 0; });
	assert(tree.size() == 4);
	assert(!tree.contains([](const int &number) { return number % 2 == 0; }));
}

void TestHetero()
{
	HeteroContainer<int> cont;
	cont.add_container(HeteroContainer<int>::STACK);
	cont.add_container(HeteroContainer<int>::LINKED_LIST);
	cont.add_container(HeteroContainer<int>::QUEUE);
	cont.add_container(HeteroContainer<int>::BIN_SEARCH_TREE);

	cont.add_element(10);
	cont.add_element(-1);
	cont.add_element(3);
	cont.add_element(4);
	cont.add_element(-4);

	int sorted[] = { -4, -1, 3, 4, 10 };
	cont.sort();
	int ind = 0;
	for (int element : cont)
	{
		assert(sorted[ind] == element);

		++ind;
	}

	cont.add_element(14);
	cont.add_element(34);
	cont.add_element(-24);
	cont.add_element(11);
	cont.add_element(4);
	cont.add_element(10);
	cont.add_element(3);
	cont.add_element(4);
	cont.add_element(-4);
	cont.add_element(14);
	cont.add_element(34);
	cont.add_element(-24);
	cont.add_element(11);
	cont.add_element(4);
	cont.add_element(-666);

	assert(cont.contains(4) == true);
	assert(cont.elements_size() == 20);

	cont.filter([](const int &number) { return number == 4; });
	assert(cont.containers_size() == 4);
	assert(cont.elements_size() == 16);
	assert(cont.contains(4) == false);
	assert(cont.contains(17) == false);

	cont.sort();
	assert(cont.containers_size() == 4);
	assert(cont.elements_size() == 16);
	assert(cont.contains(4) == false);
	assert(cont.contains(17) == false);

	assert(!(cont.get_element_it(4) != cont.end()));
	assert(cont.get_element_it(-666) != cont.end());
}

void ExecuteTests()
{
	TestStack();
	TestQueue();
	TestBinSearchTree();
	TestHetero();
}