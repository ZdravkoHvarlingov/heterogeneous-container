// HeterogeneousContainer.cpp : Defines the entry point for the console application.
//

//Needed it order to check for memory leaks
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
//-----------------------------------------

#include "Stack.h"
#include "Queue.h"
#include "DoublyLinkedList.h"
#include "HeteroContainer.h"
#include "BinSearchTree.h"

#include <iostream>
#include <fstream>

#include "Tests.h"

HeteroContainer<int> read_container(const char *fileName)
{
	std::ifstream inpFile(fileName);
	HeteroContainer<int> result;
	inpFile >> result;

	return result;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //It checks for memory leaks

	ExecuteTests();
	HeteroContainer<int> list = read_container("listExample.txt");
	std::cout << list << "\n";

    return 0;
}

