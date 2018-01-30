# heterogeneous-container
A C++ application for learning purposes utilizing the main data structures stack, queue, linked list and binary search tree. All of them combined in a single heterogeneous container. In the project are used the main object oriented programing techniques.

The heterogeneous container has the following features:
  * Adding a subcontainer - linked list, stack, queue or binary search tree;
  * Adding an element using **balanced loading** (the element is added to the container with the smallest size);
  * Checking if the container contains a specific element - directly specifying the element or using a **predicate**;
  * Filtering the container - removing all elements in alignment with a certain **predicate**;
  * Sorting all subcontainers - in the case of a binary search tree the function balances the tree;
  * **Serialization and deserialization**;
  * Iterating the container using iterators:
    * _sort iterator_ - the final result is an ascending sequence; (in the case of a binary search tree iterator uses in-order traversal);
    * _in depth iterator_ - iterates through the subcontainers one by one; (in the case of a binary search tree iterator uses pre-order traversal);
    * _in breadth iterator_ - iterates through the subcontainers layer by layer; (in the case of a binary search tree iterator uses pre-order traversal);
    
**_There are tests available for each container especially for the heterogeneous one._**
