#pragma once
#include <iostream>
//#include"Processor.h"
//class Processor;
using namespace std;
enum State { NEW = 0, RDY = 1, RUN = 2, BLK = 3, TRM = 4, ORPH = 5 };
class Process
{

private:
	int PID;
	int AT;
	int RT;
	int CT;
	int	TimeDone;
	int TimeIODuration;
	int TT;
	int	TRT;
	int WT;
	int N;
	int* IO_R;
	int* IO_D;
	int index; //for getting the IO_d data
	int IODtotal;
	int DeadLine;
	bool Forked, Child,FullForked;
	Process* Parent;
	Process* LFork;
	Process* RFork;
	//Processor* Current_processor;//For forked
	State state_;
	int _TRT_(int); // changes the TRT to get WT at any time step 
	
public:
	Process(bool = false);
	void Read(int pid, int at, int ct, int n = 0, int* ior = nullptr, int* iod = nullptr, int dead = 0);
	void setResponsetime(int CPU_FIRST_TIME);
	int getResponsetime();
	bool Step();

	int getAT();
	int getPID();
	int getRT();
	int getCT();
	int getTT();
	int getTRT();
	int getWT(int = 0);
	int getN();
	int GetTotalIOD();


	int getDeadline();
	void setDeadline(int);

	//all of that is for Forked
	bool isChild();
	bool isForked();//returns true if it is forked
	bool isTOTForked();

	Process* getLeftFork();
	Process* getRightFork();
	Process* getParent();

	void removeChild(Process*&);
	void setParent(Process*);
	bool setFork(Process*);

	//Processor* getProcessor();
	//void setProcessor(Processor*);
	////
	int getTimeIODuration();
	int getTimeDone();
	bool GetIO_R(int& IOR);
	bool GetIO_D(int& IOD);
	void UpdateIO();
	void Terminated(int& T);
	void Blocked();
	void print();
	State getState();
	void setState(State S);

	~Process();
};

