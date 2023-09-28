#pragma once

#include<windows.h>
#include"Queue.h"
#include"Process.h"
#include<ctime>
#include<cstdlib>
enum States
{
	Busy, Idle
};
class Processor
{
private:
	States state;
	
	int FinishTime;
	Process* RUNP;
	string Name;
protected:
	int RDYSize;
	int Busytime;
	int Idletime;
	int IOTimer;
	int OverHeat;
	bool Over;
	int OverheatingCounter;
	void setSize(int R)
	{
		RDYSize = R;
	}
	void setName(string s) {
		Name = s;
	}
	void setFinishTime(int s)
	{
		FinishTime = s;
	}
public:
	Processor() {
		this->state = Idle;  //by Ahmed Kamal
		Busytime = 0;
		Idletime = 0;
		FinishTime = 0;
		RDYSize = 0;
		this->IOTimer = 0;
		RUNP = nullptr;
		Over = false;

	}
	virtual void AddProcess(Process*) = 0;
	//untouchable
	virtual void print() = 0;
	//untouchable
	virtual void PeekProcess(Process*& ptr) = 0;
	virtual bool removePeek(Process*&) = 0;
	virtual int SchedulingAlgorithm(Process*& ptr, int Time) = 0;

	virtual bool RemoveRun(Process*&) = 0;
	
	virtual bool BLKstep(Process*&) = 0;

	bool isOverHeat() {
		return this->Over;
	}
	void MakeOverHeat(bool o) {
		this->Over = o;
	}
	int getOverHeatTime() {
		return this->OverHeat;
	}
	void setOverHeatTime(int o) {
		this->OverHeat = o;
	}

	float ProcessorUtilization()
	{
		return ((float)Busytime / (Busytime + Idletime))*100;
	}
	float ProcessorLoad(int TotalTRT)
	{
		return ((float)this->Busytime / TotalTRT)*100;
	}
	 
	virtual States getstate()
	{
		return state;
	}
	void setstate(States s)
	{
		state=s;
	}
	void setRUNP(Process* S)
	{
		RUNP = S;
	}
	virtual Process* getRUNP()
	{
		return RUNP;
	}
	int getBusyTime()
	{
		return Busytime;
	}
	int getSize()
	{
		return RDYSize;
	}
	string getName() {
		return Name;
	}
	int getFinishTime()
	{
		return FinishTime;
	}
};
