#pragma once
#include "Processor.h"
#include"Process.h"
#include<iostream>

using namespace std;
class RR_Processor :public Processor
{
private:
	//untouchable
	Queue<Process*> RDYList;
	//untouchable
	int TimeSlice;
	double RTF;
public:
	RR_Processor(double Rtf, int TS,int overheat)
	{
		RTF = Rtf;
		setstate(Idle);
		TimeSlice = TS;
		setName("[RR  ]");
		setSize(0);
		setFinishTime(0);
		Busytime = 0;
		Idletime = 0;
		this->OverHeat = overheat;
		this->Over = false;
		this->OverheatingCounter = 0;
		setRUNP(nullptr);
	}
	void setTimeSlice(int TS) {
		this->TimeSlice = TS;
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
				return 0;
			}
			return 0;
		}
		return 0;

	}
	
	int SchedulingAlgorithm(Process*& ptr, int Time)
		// 0||-1=> No returned ptr
		// 1=> to BLK
		// 2 => To termination
		// 3=> To forking

	{
		//if idle do nothing
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
					if ((ptr->getCT() - ptr->getTimeDone()) % TimeSlice == 0)
					{
						this->RemoveRun(ptr);
						this->AddProcess(ptr);//Not Sure it should stay in the same Processor's RDY or Put in the shortest.
						return 0;
					}
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
		if (this->getRUNP()==nullptr && this->RDYSize == 0) {//add it to the run
			this->setRUNP(S);
		}
		else {
			RDYList.enqueue(S);
			this->RDYSize++;
		}
		//to set it busy
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
	bool ProcessMigration(Process*& ptr, int size)
	{
		if (getRUNP() == nullptr || size == 0)
			return false;
		else if (getRUNP()->getTimeDone() < RTF && getRUNP()->isChild() == false)
		{
			RemoveRun(ptr);
			return true;
		}
		else
			return false;

	}
	virtual bool RemoveRun(Process*& ptr) {
		ptr = this->getRUNP();
		if (ptr) {// if there is something in run
			setFinishTime(getFinishTime() -  ptr->getTimeDone());
			Process* theNew;//to make the front of the Ready list move to the run
			if (this->RDYList.dequeue(theNew) && RDYSize != 0) {
				this->RDYSize--;
				this->setstate(Busy);
				this->setRUNP(theNew);
			}
			else {
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
			Ptr = nullptr; //added Ahmed Kamal 
			return false; //added
		}
		else
		{
			RDYList.dequeue(Ptr);
			setSize(getSize() - 1);
			setFinishTime(getFinishTime() - Ptr->getTimeDone());
			return true;
		}
	}
	void PeekProcess(Process*& ptr)
	{
		ptr = RDYList.peekFront();
	}
};