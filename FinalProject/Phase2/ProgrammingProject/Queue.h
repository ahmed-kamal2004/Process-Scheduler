#pragma once
#include"Process.h"
template<class T>
class Node
{
private:
	T Data;
	Node<T>* Next;
public:
	Node() {
		Next = nullptr;
		Data = NULL;
	}
	void SetData(const T& D) {
		Data = D;
	}
	void SetNext(Node<T>* N) {
		Next = N;
	}
	T GetData() {
		return this->Data;
	}
	Node<T>* GetNext() {
		return this->Next;
	}

};




#include<iostream>
using namespace std;
template<typename T>
class Queue
{
protected:
	Node<T>* Front;
	Node<T>* Rear;
	int counter;
public:
	Queue() {
		this->Front = nullptr;
		this->Rear = nullptr;
		counter = 0;
	}
	Queue(const Queue& other) : Front(nullptr), Rear(nullptr)
	{
		Node<T>* temp = other.Front;
		while (temp != nullptr) {
			enqueue(temp->GetData());
			temp = temp->GetNext();
		}
	}
	virtual bool isEmpty() const {

		return ((this->Front == nullptr) && (this->Rear == nullptr));
	}
	void operator =(Queue<T>Q) {//assigment operator
		T  Ptr;
		while (this->dequeue(Ptr)) {
		}
		while (Q.dequeue(Ptr)) {
			this->enqueue(Ptr);
		}
	}

	virtual bool enqueue(const T& item) {
		//if (item == NULL)//added to handle adding nullptr
		//	return true;
		Node<T>* New = new Node<T>;
		New->SetData(item);
		New->SetNext(nullptr);
		if (this->isEmpty()) {
			this->Front = this->Rear = New;
			return true;
		}
		this->Rear->SetNext(New);
		Rear = New;
		return true;

	}
	virtual bool dequeue(T& Ptr) {//here I dont want to make deletion I just want to return the process without deleting it 
		if (this->isEmpty())
			return false;
		if (this->Front == this->Rear)
		{
			Ptr = this->Front->GetData();
			delete this->Front;
			this->Rear = nullptr;
			this->Front = nullptr;
			return true;
		}
		Ptr = this->Front->GetData();
		Node<T>* deleter = this->Front;
		this->Front = this->Front->GetNext();
		delete deleter;
		return true;
	}
	virtual T peekFront()const {
		if (this->isEmpty())
			return NULL;
		return this->Front->GetData();
	}
	~Queue() {

		T Ptr;
		while (this->dequeue(Ptr)) {
		}
	}
	void print();
};
template<typename T>
inline void Queue<T>::print()
{
	Node<T>* temp = this->Front;
	while (temp != NULL)
	{
		temp->GetData()->print();
		cout << " , ";
		temp = temp->GetNext();
	}
}

template<>
inline void Queue<Process*>::print() {
	Node<Process*>* temp = this->Front;
	while (temp != NULL)
	{
		temp->GetData()->print();
		cout << " , ";
		temp = temp->GetNext();
	}
}


class FCFS_Queue :public Queue<Process*> {//I added functions print remove and search using PId
public:
	bool search(int PID,int &index){//these function removes process from the FCFS_Queue list  // you must outside decrement the size of the lisrt and the finish time
		index = 0;
		Node<Process*>*Looper= Queue<Process*>::Front;
		while (Looper != nullptr&& Looper->GetData()!=nullptr) {

			if (Looper->GetData()->getPID() == PID) {
				return true;
			}
			index++;
			Looper = Looper->GetNext();
		}
		return false;
	}
	
	bool searchNotForked(int& index) {//these function removes process from the FCFS_Queue list  // you must outside decrement the size of the lisrt and the finish time

		Node<Process*>* Looper = Queue<Process*>::Front;
		while (Looper != nullptr && Looper->GetData() != nullptr) {

			if (!Looper->GetData()->isChild()) {
				return true;
			}
			index++;
			Looper = Looper->GetNext();
		}
		return false;

	}

	bool remove(int index, Process*& ptr) {
		ptr = nullptr;
		if (this->isEmpty()) {
			return false;
		}
		if (index==0)
		{
			this->dequeue(ptr);
			return true;
		}
		Node<Process*>* Looper = this->Front->GetNext();
		Node<Process*>* Prev = this->Front;
		int counter = 1;
		while (Looper != nullptr) {
			if (counter==index) {
				if (Looper->GetNext() == nullptr)
					this->Rear = Prev; //to  handle tha last one 
				Prev->SetNext(Looper->GetNext());
				ptr = Looper->GetData();
				delete Looper;
				return true;
			}
			Prev = Looper;
			Looper = Looper->GetNext();
			counter++;
		}
		return false;
	}
};