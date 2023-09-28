#pragma once
#include "Heap_implementation.h"
#include"Process.h"
#include<iostream>
using namespace std;

class MinPriority_Queue :
    protected MinHeap_implementation<int, Process*>
{
public:
    MinPriority_Queue();
    MinPriority_Queue(int size);

    MinPriority_Queue(MinPriority_Queue&);
    bool isEmpty() const;
    bool add(const int& priority,  Process*& content);
    bool remove();
    void print();
    Data_BLock<int, Process*> peek() const;
};

inline MinPriority_Queue::MinPriority_Queue()
{
    MinHeap_implementation<int, Process*>::MinHeap_implementation();
}

inline MinPriority_Queue::MinPriority_Queue(int size)
{
    MinHeap_implementation<int, Process*>::MinHeap_implementation(size);
}


inline MinPriority_Queue::MinPriority_Queue(MinPriority_Queue& PriQueue)
{
    MinHeap_implementation<int, Process*>::MinHeap_implementation(PriQueue);
}

inline bool MinPriority_Queue::isEmpty() const
{
    return MinHeap_implementation<int, Process*>::isEmpty();
}

inline bool MinPriority_Queue::add(const int & priority,  Process*& content)
{
    return MinHeap_implementation<int, Process*>::add(priority, content);
}

inline bool MinPriority_Queue::remove()
{
    return MinHeap_implementation<int, Process*>::remove();
}

inline void MinPriority_Queue::print()
{
    for (int Looper = 0; Looper < this->itemCount; Looper++) {
        this->items[Looper].getContent()->print();
        cout << " , ";
    }
}



inline Data_BLock<int, Process*> MinPriority_Queue::peek() const
{
    return MinHeap_implementation<int, Process*>::peekTop();
}
