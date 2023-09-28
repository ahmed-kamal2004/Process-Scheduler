#pragma once
#include "Processor.h"
#include"Process.h"
#include"Priority_Queue.h"
#include<iostream>
using namespace std;
class SJF_Processor :public Processor
{
private:
	//untouchable
	MinPriority_Queue RDYList;
	//untouchable
public:
	SJF_Processor(int overheat)
	{
		setName("[SJF ]");
		setstate(Idle);
		setSize(0);
		setFinishTime(0);
		Busytime = 0;
		Idletime = 0;
		this->OverHeat = overheat;
		this->Over = false;
		this->OverheatingCounter=0;
		setRUNP(nullptr);

	}
	bool BLKstep(Process*& ptr) {
		ptr = this->getRUNP();
		if (getstate() == Busy && !this->isOverHeat())
		{
			int IORT;
			if (this->getRUNP()->GetIO_R(IORT))
			{
				if (this->IOTimer == IORT)
				{
					this->RemoveRun(ptr);
					ptr->Blocked();
					this->IOTimer = 0;
					return 1;
				}
				this->IOTimer++;
			}
		}
		return 0;
	}
	int SchedulingAlgorithm(Process*& ptr, int Time)
		// 0=> No returned ptr
		// 1=> to BLK
		// 2 => To termination


	{
		if (this->isOverHeat()) {
			if (this->OverheatingCounter == this->OverHeat) {
				this->OverheatingCounter = 0;
				this->MakeOverHeat(false);
				this->setstate(Idle);
			}
			else {
				this->OverheatingCounter++;
			}
			return -1;
		}
		else {
			//if idle do nothing
			ptr = this->getRUNP();
			if (getstate() == Idle)
			{
				this->Idletime++;
				return -1;
			}
			else if (getstate() == Busy)
			{
				int x = rand() % 100;
				if (x < 2) {
					this->MakeOverHeat(true);
				}
				this->Busytime++;
				if (this->getRUNP()->Step())
				{
					setFinishTime(getFinishTime() - 1);
				}
				else
				{
					this->RemoveRun(ptr);
					if (this->getstate() == Busy)
					{
						this->getRUNP()->setResponsetime(Time);
					}

					return 2;
				}
				return 0;
			}
		}
		return 0;
	}
	void AddProcess(Process* S)
	{
		if (S == nullptr) {
			return;
		}
		if (this->getRUNP()==nullptr && this->RDYSize == 0) 
		{//add it to the run
		
			this->setRUNP(S);
		}
		else 
		{
			RDYList.add(S->getCT(),S);
			this->RDYSize++;
		}
		setstate(Busy);
		setFinishTime(S->getTimeDone() + getFinishTime());
	}

	//untouchable
	void print()
	{
		cout << this->getName() << " " << (this->isOverHeat() ? "Overheated !" : " ") << " " << this->getSize() << " RDY ";
		RDYList.print();
		cout << endl;
	}
	//untouchable

	virtual bool RemoveRun(Process*& ptr) {
		ptr = this->getRUNP();
		if (ptr) {// if there is something in run
			setFinishTime(getFinishTime() - ptr->getTimeDone());
			Process* theNew= this->RDYList.peek().getContent();
			//to make the front of the Ready list move to the run
			
			if (theNew!=nullptr&&RDYSize!=0) 
			{
				this->RDYList.remove();
				this->RDYSize--;
				this->setstate(Busy);
				this->setRUNP(theNew);
			}
			else 
			{
				this->setstate(Idle);
				setRUNP(nullptr);
			}
			return true;
		}
		this->setstate(Idle);
		setRUNP(nullptr);
		return false;///if there is nothing found in the run
	}
	bool removePeek(Process*& Ptr) {
		if (RDYList.isEmpty()) {
			setstate(Idle);
			Ptr = nullptr;
			return false;
		}
		else
		{
			Ptr = RDYList.peek().getContent();
			RDYList.remove();
			setSize(getSize() - 1);
			setFinishTime(getFinishTime() - Ptr->getTimeDone());
			return true;
		}
	}
	void PeekProcess(Process*& ptr)
	{
		if (this->RDYSize == 0) {
			ptr = nullptr;
			return;
		}
		ptr = RDYList.peek().getContent();
	}

};
