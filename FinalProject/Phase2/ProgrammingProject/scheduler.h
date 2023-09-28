#include"Priority_Queue.h"
#include"FCFS_Processor.h"
#include"RR_Processor.h"
#include"SJF_Processor.h"
#include"EDF_Processor.h"
#include<iostream>
#include"UIclass.h"
#include<string>
using namespace std;
//#define ABS_PATH "C:\\Users\\ahmed\\Desktop\\New folder\\Project2\\" //THIS NEED TO BE CHANGED TO MATCH THE DIRECTORY OF INPUT FILE. BY:~A.
#define ABS_PATH "..\\"
class scheduler
{
private:
	//process * scheduler_process
	//"PLEASE CHANGE THE TYPE OF LISTS MAKE EACH83 OF ITS PROCESSOR TYPE !!!" BY ~A.
	Processor** RR_processor;
	Processor** FCFS_processor;
	Processor** SJF_processor;
	Processor** EDF_processor;
	//new list   queue
	//trm list   queue
	//blk list   queue
	Queue<Process*>NEW;
	Queue<Process*>BLK;
	Queue<Process*>TRM;



	Queue<Process*>WAIT_FCFS;
	Queue<Process*>WAIT_SJF;
	Queue<Process*>WAIT_RR;
	Queue<Process*>WAIT_EDF;
	Queue<Process*>WAIT_;
	//for sigkill
	Queue<int>Sigkill_Time;
	Queue<int>Sigkill_PID;

	UIclass* UI;
	//Queue<Process*> SIGKILL; //Added by ~A.
	int Mode;

	int FCFS_size, SJF_size, RR_size, EDF_size, RR_Time_slice, RTF, MaxW, STL, no_of_processes, Time_step, BLK_counter, OverHeat,WAIT_size,WAITRR_size,WAITSJF_size,Finished_before_deadline;

	double  Fork_probability, AvgWt, AvgRt, AvgTRT, Migration_RTf,
		Migration_MaxW, WorkSteal, ForkedProcess, KilledProcess, AvgUtilization;

	int TRMsize, NEWsize, BLKsize;

	void WAITtoPRO();
	void NewToshortest();
	void RunToBlk();
	void BlkTShortest();
	void ProcessMigrationSimulator();
	void LoopingProcessors(MinPriority_Queue& runs, int& runsize);
	void SolveOverHeating();
	void KillingSignal();
	bool fileRead(string File = "inputFile");
	void fileWrite(string File = "outputFile");
	void workStealing();
	void deleteForked(Process*);
	void TermenateForked(Process*);
	Processor* shortest();
	Processor* shortestRR();
	Processor* shortestFCFS();
	Processor* shortestSJF();
	Processor* shortestEDF();
	Processor* longest();
	Processor* longestRR();
	Processor* longestFCFS();
	Processor* longestSJF();
	Processor* longestEDF();
	bool KillProcess(int ID);
	void To_BLK(Process*& scheduldedProcess);
	void To_TRM(Process*& scheduldedProcess);


public:
	scheduler();

	//main function
	void simulator(string file,string outFile);


	~scheduler();
};