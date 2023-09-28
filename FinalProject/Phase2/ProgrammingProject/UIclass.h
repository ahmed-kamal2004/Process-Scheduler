#pragma once
#include"Processor.h"
#include"Process.h"
#include"FCFS_Processor.h"
#include"RR_Processor.h"
#include"SJF_Processor.h"
class UIclass
{
private:
	int Mode;
	void PrintTIME(int& Time);
	void RDY_STATE();
	void BLK_STATE();
	void PrintBLK(Queue<Process*>&, int);
	void RUN_STATE();
	void PrintRUN(MinPriority_Queue&,int size);
	void WAIT_STATE();
	void PrintWAIT(Queue<Process*>&, int size);
	void WAITRR_STATE();
	void PrintWAITRR(Queue<Process*>&, int size);
	void WAITSJF_STATE();
	void PrintWAITSJF(Queue<Process*>&, int size);
	void TRM_STATE();
	void PrintTRM(Queue<Process*>&, int);
	void WAIT();
public:
	void PrintMode();
	 void GetMode();
	 void Printer(int,Processor**, Processor**, Processor**, Processor**,int,int,int,int, Queue<Process*>&, int, Queue<Process*>&, int, MinPriority_Queue&,int, Queue<Process*>&, int, Queue<Process*>&, int, Queue<Process*>&, int);
	// void Printer(int, Processor*, Processor*, Processor*, int, int, int, Queue<Process*>&, int, Queue<Process*>&, int);
	 /*
	 inputs :
		time

		FCFS list
		RR list
		SJF list

		FCFS size
		RR size
		SJF size

		BLK 
		,BLKsize
		TRM
		,TRM size
	 */


	 void End();
};

