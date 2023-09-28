#pragma once
#include"Process.h"
#include"Processor.h"
#include"Queue.h"
#include<iostream>

using namespace std;
class FCFS_Processor :public Processor
{
private:
	FCFS_Queue RDYList;
	double ForkProbability;
	double MAXW;
public:
	FCFS_Processor(double forkProbability, double MaxW ,int overheat)
	{
		ForkProbability = forkProbability;
		MAXW = MaxW;
		setName("[FCFS]");
		setstate(Idle);
		setSize(0);
		setFinishTime(0);
		Busytime = 0;
		Idletime = 0;
		this->OverHeat = overheat;
		this->Over = false;
		this->OverheatingCounter = 0;
		setRUNP(nullptr);
	}
	bool BLKstep(Process*& ptr) {
		ptr = this->getRUNP();
		if (getstate() == Busy && !this->isOverHeat())
		{
			int IORT;
			if (this->getRUNP()->GetIO_R(IORT) && 
				!this->getRUNP()->isChild() && !this->getRUNP()->isForked()) {

				if (this->IOTimer == IORT) {
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
		// 3=> To forking

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

					//no need to handle the forked because it does not have IO
					//here you work on BLK and process migration and nothing
				{
					setFinishTime(getFinishTime() - 1);

					//Forking
					//make probability
					int y = rand() % 100;
					if ((y > 0 && y < this->ForkProbability) && !this->getRUNP()->isTOTForked()) {
						return 3;
					}
					//
					return 0;

				}
				else
				{
					this->RemoveRun(ptr);
					if (this->getstate() == Busy)
					{
						this->getRUNP()->setResponsetime(Time);
					}
					return 2; //to termination
				}
				return -1;
			} 
			return -1;
		}
		return -1;
	}


	//used for adding process and handiling every thing
	void AddProcess(Process* S)
	{
		if (S == nullptr) {
			return;
		}
		if (this->getRUNP() == nullptr && this->RDYSize == 0) {//add it to the run
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
	bool ProcessMigration(Process*& ptr, int size, int time)
	{
		if (getRUNP() == nullptr || size == 0)
			return false;
		else if (getRUNP()->getWT(time) > MAXW && getRUNP()->isForked() == false  &&getRUNP()->isChild()==false)
		{
			this->RemoveRun(ptr);
			return true;
		}
		else
			return false;

	}
	//untouchable
	void print()
	{
		cout << this->getName() << " " << (this->isOverHeat()?"Overheated !":" ") << " " << this->getSize() << " RDY ";		RDYList.print();
		cout << endl;
	}
	//untouchable


	//used for killing process
		//used for killing process
	bool Kill_FCFS_Process(int ID, Process*& ptr)
	{
		int index;
		ptr = nullptr;
		bool b = RDYList.search(ID, index);
		if (b)
		{
			RDYList.remove(index, ptr);
			this->RDYSize--;
			setFinishTime(getFinishTime() - ptr->getTimeDone());
			return true;
		}
		ptr = this->getRUNP();
		if (ptr)
		{
			if (ptr->getPID() == ID)
			{
				this->RemoveRun(ptr);
				return true;
			}

		}
		ptr = nullptr;
		return false;
	}


	//used for removing the RUn process and handling every thing about that in   //used in process Killing
	virtual bool RemoveRun(Process*& ptr) {
		ptr = this->getRUNP();
		if (ptr) {// if there is something in run
			setFinishTime(getFinishTime() - ptr->getTimeDone());
			Process* theNew;//to make the front of the Ready list move to the run
			if (this->RDYList.dequeue(theNew)) {
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

	//need to be worked on it 
	bool removePeek(Process*& Ptr)
	{

		if (RDYList.isEmpty()) {
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
	void stealNotForked(int & index,Process*& ptr)
	{
		ptr = nullptr;
		if (this->RDYList.searchNotForked(index))
		{

			this->RDYSize--;
			if (!this->getRUNP() && !this->RDYSize)
				this->setstate(Idle);
			if (RDYList.remove(index, ptr))
				setFinishTime(getFinishTime() - ptr->getTimeDone());
			return;
		}
		
	}
};
