#pragma once
#include<iostream>
#include"Process.h"
//#include "Heap_ADT.h"
#define MAxi 100000
template< class ItemType1, class ItemType2>
class Data_BLock {
	ItemType1 priority;
	ItemType2 content;
public:
	Data_BLock() {
		this->priority = NULL;
		this->content = NULL;
	}
	void operator =(Data_BLock<ItemType1, ItemType2>D) {
		this->priority = D.priority;
		this->content = D.content;
	}
	void setPriority(ItemType1 P) {
		this->priority = P;
		return;
	}
	void setContent(ItemType2 C) {
		this->content = C;  //here we need copy constructor ans assignment operator
		return;
	}
	ItemType1 getPriority() {
		return this->priority;
	}
	ItemType2 getContent() {
		return this->content;
	}


};



template< class ItemType1, class ItemType2>
class MinHeap_implementation
{
protected:
	Data_BLock<ItemType1, ItemType2>* items;
	int itemCount;
private:

	//static const int ROOT_INDEX = 0;


	int maxItems;


	// Returns the array index of the left child (if it exists).
	int getLeftChildIndex(const int nodeIndex) const;
	// Returns the array index of the right child (if it exists).
	int getRightChildIndex(int nodeIndex) const;
	// Returns the array index of the parent node.
	int getParentIndex(int nodeIndex) const;
	// Tests whether this node is a leaf.
	bool isLeaf(int nodeIndex) const;
protected:
	// Converts a semiheap to a heap.
	void heapRebuild(int subTreeRootIndex);
	// Creates a heap from an unordered array.
	void heapCreate();


public:
	MinHeap_implementation();
	MinHeap_implementation(int);
	MinHeap_implementation(const Data_BLock<ItemType1, ItemType2> someArray[], const int arraySize);
	MinHeap_implementation(MinHeap_implementation<ItemType1, ItemType2>&);
	virtual bool isEmpty() const;

	virtual int getNumberOfNodes() const;

	//virtual int getHeight() const;

	virtual Data_BLock<ItemType1, ItemType2> peekTop() const;

	virtual bool add(const ItemType1& priority, const ItemType2& content);

	virtual bool remove();

	virtual void clear();


	virtual ~MinHeap_implementation();

};
template< class ItemType1, class ItemType2>
inline int MinHeap_implementation<ItemType1, ItemType2>::getLeftChildIndex(const int nodeIndex) const
{
	return (2 * nodeIndex) + 1;
}
template< class ItemType1, class ItemType2>
inline int MinHeap_implementation<ItemType1, ItemType2>::getRightChildIndex(int nodeIndex) const
{
	return (2 * nodeIndex) + 2;
}
template< class ItemType1, class ItemType2>
inline int MinHeap_implementation<ItemType1, ItemType2>::getParentIndex(int nodeIndex) const
{
	return ((nodeIndex - 1) / 2);
}
template< class ItemType1, class ItemType2>
inline bool MinHeap_implementation<ItemType1, ItemType2>::isLeaf(int nodeIndex) const
{
	if (this->getLeftChildIndex(nodeIndex) > this->itemCount && this->getRightChildIndex(nodeIndex) > this->itemCount)
		return true;// this is my assumption Maybe true maybe false thanks stupid
	return false;
}
template< class ItemType1, class ItemType2>
inline void MinHeap_implementation<ItemType1, ItemType2>::heapRebuild(int subTreeRootIndex)
{
	if (this->isLeaf(subTreeRootIndex))
		return;
	int smaller = this->getLeftChildIndex(subTreeRootIndex);//smaller in our case must be handeled
	if (this->getRightChildIndex(subTreeRootIndex) < this->itemCount) {
		int rightIndex = smaller + 1;
		if (this->items[rightIndex].getPriority() < this->items[smaller].getPriority())
		{
			smaller = rightIndex;
		}
	}

	//swaping
	if (this->items[subTreeRootIndex].getPriority() > this->items[smaller].getPriority()) {
		Data_BLock<ItemType1, ItemType2> data;
		data = this->items[subTreeRootIndex];
		this->items[subTreeRootIndex] = this->items[smaller];
		this->items[smaller] = data;
		this->heapRebuild(smaller);
	}
	return;
}
template< class ItemType1, class ItemType2>
inline void MinHeap_implementation<ItemType1, ItemType2>::heapCreate()
{
	for (int i = this->itemCount - 1; i >= 0; i--) {
		this->heapRebuild(i);
	}
	return;

}
template< class ItemType1, class ItemType2>
inline MinHeap_implementation<ItemType1, ItemType2>::MinHeap_implementation()
{
	this->maxItems = MAxi;
	this->items = new Data_BLock<ItemType1, ItemType2>[MAxi];
	this->itemCount = 0;
}
template< class ItemType1, class ItemType2>
inline MinHeap_implementation<ItemType1, ItemType2>::MinHeap_implementation(int MAX)
{
	this->maxItems = MAX;
	this->items = new Data_BLock<ItemType1, ItemType2>[MAX];
	this->itemCount = 0;
}

template< class ItemType1, class ItemType2>
inline MinHeap_implementation<ItemType1, ItemType2>::MinHeap_implementation(const Data_BLock<ItemType1, ItemType2> someArray[], const int arraySize) :itemCount(arraySize), maxItems(2 * arraySize)
{
	for (int i = 0; i < arraySize; i++) {
		this->items[i] = someArray[i];
	}
	this->heapCreate();
}

template<class ItemType1, class ItemType2>
inline MinHeap_implementation<ItemType1, ItemType2>::MinHeap_implementation(MinHeap_implementation<ItemType1, ItemType2>& Heap)
{
	this->maxItems = Heap.maxItems;
	this->items = new Data_BLock<ItemType1, ItemType2>[Heap.maxItems];
	this->itemCount = 0;
	for (int looper = 0; looper < Heap.itemCount; looper++) {
		this->add(Heap.items[looper].getPriority(), Heap.items[looper].getContent());
	}
}

template< class ItemType1, class ItemType2>
inline bool MinHeap_implementation<ItemType1, ItemType2>::isEmpty() const
{
	return !this->itemCount;
}

template< class ItemType1, class ItemType2>
inline int MinHeap_implementation<ItemType1, ItemType2>::getNumberOfNodes() const
{
	return this->itemCount;
}

template< class ItemType1, class ItemType2>
inline Data_BLock<ItemType1, ItemType2> MinHeap_implementation<ItemType1, ItemType2>::peekTop() const
{
	return this->items[0];
}

template< class ItemType1, class ItemType2>
inline bool MinHeap_implementation<ItemType1, ItemType2>::add(const ItemType1& priority, const ItemType2& content)
{
	if (this->itemCount == this->maxItems)
		return false;
	this->items[this->itemCount].setPriority(priority);//setters
	this->items[this->itemCount].setContent(content);

	int Temp_counter = this->itemCount;
	bool inplace = false;
	while (Temp_counter >= 0 && !inplace) {
		int parent = (Temp_counter - 1) / 2;
		if (this->items[parent].getPriority() > this->items[Temp_counter].getPriority()) {
			Data_BLock<ItemType1, ItemType2> data = this->items[parent];
			this->items[parent] = this->items[Temp_counter];
			this->items[Temp_counter] = data;
			Temp_counter = parent;
		}
		else {
			inplace = true;
		}

	}
	this->itemCount++;
	return true;
}

template< class ItemType1, class ItemType2>
inline bool MinHeap_implementation<ItemType1, ItemType2>::remove()
{
	this->items[0] = this->items[--this->itemCount];
	this->heapRebuild(0);
	return true;
}

template< class ItemType1, class ItemType2>
inline void MinHeap_implementation<ItemType1, ItemType2>::clear()
{
	this->itemCount = 0;
	return;
}

template< class ItemType1, class ItemType2>
inline MinHeap_implementation<ItemType1, ItemType2>::~MinHeap_implementation()
{
}
