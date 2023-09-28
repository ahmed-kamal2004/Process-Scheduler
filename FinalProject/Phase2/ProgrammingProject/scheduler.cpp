#include "scheduler.h"
#include<iomanip>
#include<fstream>
#include <filesystem>
using namespace std;
scheduler::scheduler()
{
	//srand(time(NULL));
	this->FCFS_size = this->SJF_size = this->RR_size = this->RR_Time_slice
		= this->RTF = this->MaxW = this->STL = this->no_of_processes =
		this->Time_step = this->Fork_probability = this->AvgWt = this->AvgRt =
		this->AvgTRT = this->Migration_RTf = this->Migration_MaxW = this->WorkSteal =
		this->ForkedProcess = this->KilledProcess = this->AvgUtilization = this->BLKsize =
		this->NEWsize = this->TRMsize = this->Mode = this->BLK_counter = this->EDF_size = this->OverHeat =this->WAITRR_size=this->WAITSJF_size=this->WAIT_size= Finished_before_deadline= 0;
	UI = new UIclass;
	UI->GetMode();
}

bool scheduler::fileRead(string File)
{
	ifstream file;
	file.open(ABS_PATH + File + ".txt", ios::in);
	if (!file.is_open()) {//checking for opening file
		std::cerr << "Error: Unable to open file\nHere some solutions :\n1.check the file name in the source code \"in Source.cpp file\".\n2.check the Absolute path of the File among your PC then,\n\t - copy that File path and paste it in schedular.h file \"in ABS_Path varible defination\".\n\n****   Hope these solutions helped you :)  **** " << std::endl;
		return false;
	}
	file >> FCFS_size >> SJF_size >> RR_size>>EDF_size;
	file >> RR_Time_slice>>OverHeat;

	this->FCFS_processor = new Processor * [this->FCFS_size];//is abstract class ahmed mostafa should complete it
	this->RR_processor = new Processor * [this->RR_size];//that is used to make an array of RR processors that dont have default constructor
	this->SJF_processor = new Processor * [this->SJF_size];
	this->EDF_processor = new Processor * [this->EDF_size];//is abstract class ahmed mostafa should complete it
	for (int Looper = 0; Looper < this->SJF_size; Looper++) {
		this->SJF_processor[Looper] = new SJF_Processor(OverHeat);
	}
	for (int Looper = 0; Looper < this->EDF_size; Looper++) {
		this->EDF_processor[Looper] = new EDF_Processor(OverHeat);
	}

	file >> RTF >> MaxW >> STL >> Fork_probability;
	for (int Looper = 0; Looper < this->RR_size; Looper++)
	{
		this->RR_processor[Looper] = new RR_Processor(RTF, RR_Time_slice, OverHeat);
	}
	for (int Looper = 0; Looper < this->FCFS_size; Looper++) {
		this->FCFS_processor[Looper] = new FCFS_Processor(Fork_probability, MaxW, OverHeat);
	}
	file >> no_of_processes;
	int AT, PID, CT, N,dead;
	string IO;
	for (int i = 0; i < this->no_of_processes; i++) {
		file >> AT >> PID >> CT >> dead >> N;
		int* IO_R = new int[N];
		int* IO_D = new int[N];
		getline(file, IO);
		int start = 0;
		int ind = 0;

		while ((ind < N) && (start < IO.length())) {
			// Find the start and end positions of the next tuple
			int first = IO.find('(', start);
			int second = IO.find(',', first);
			int third = IO.find(')', second);

			// Extract the values from the tuple and convert them to integers
			IO_R[ind] = stoi(IO.substr(first + 1, second - first - 1));
			IO_D[ind] = stoi(IO.substr(second + 1, third - second - 1));
			ind++;
			start = third + 1;
		}

		//adding to the process list
		Process* new_process = new Process;
		new_process->Read(PID, AT, CT, N, IO_R, IO_D,dead);
		NEW.enqueue(new_process);
		this->NEWsize++;
		if (N)
		{
			delete[N]IO_R;
			delete[N]IO_D;
		}
	}

	//reading sigkilling data

	//need to complete sigkill input data but it is under construction in phase 2
	int time_step_sigkill, Pid_sigkill;
	while (file >> time_step_sigkill >> Pid_sigkill) {
		// Enqueue the time and PID values
		this->Sigkill_Time.enqueue(time_step_sigkill);
		this->Sigkill_PID.enqueue(Pid_sigkill);
	}

	file.close();
	return true;
}

void scheduler::fileWrite(string File)
{
	ofstream file;
	file.open(ABS_PATH + File + ".txt", ios::out);
	//perror("the Error");
	int k;
	//loop over the trm list 
	file << "TT        PID        AT        CT        IO_D        WT      RT      TRT\n";
	Process* PrintedProcess;
	while (this->TRM.dequeue(PrintedProcess)) {
		file <<PrintedProcess->getTT();
		file << std::setw(10) << PrintedProcess->getPID();
		file << std::setw(10) << PrintedProcess->getAT();
		file << std::setw(10) << PrintedProcess->getCT();
		file << std::setw(10) << PrintedProcess->GetTotalIOD();//need to be completed
		file << std::setw(10) << PrintedProcess->getWT();
		file << std::setw(10) << PrintedProcess->getResponsetime();
		file << std::setw(10) << PrintedProcess->getTRT();
		file << endl;
		AvgWt += PrintedProcess->getWT();
		AvgRt += PrintedProcess->getRT();
		AvgTRT += PrintedProcess->getTRT();
	}
	file << "\n";
	file << "Processes: " << this->no_of_processes << "\n";
	///// 
	float avgtrt = AvgTRT;
	AvgWt = AvgWt / no_of_processes;
	AvgRt = AvgRt / no_of_processes;
	AvgTRT = AvgTRT / no_of_processes;
	Migration_MaxW = (Migration_MaxW / no_of_processes) * 100;
	Migration_RTf = (Migration_RTf / no_of_processes) * 100;
	WorkSteal = (WorkSteal / no_of_processes) * 100;
	ForkedProcess = (ForkedProcess / no_of_processes) * 100;
	KilledProcess = (KilledProcess / no_of_processes) * 100;
	Finished_before_deadline = ((float)Finished_before_deadline / no_of_processes) * 100;
	/////


	file << "Avg WT = " << this->AvgWt << "      Avg RT = " <<
		this->AvgRt << "      Avg TRT = " << this->AvgTRT << "\n";
	file << "Migration %:     RTF= " << this->Migration_RTf << "%,      MaxW= " << this->Migration_MaxW << "%\n";
	file << "Work Steal%: " << this->WorkSteal << "%\n";
	file << "Forked Process: " << this->ForkedProcess << "%\n";
	file << "Killed Process: " << this->KilledProcess << "%\n";
	file << "Processes Finished before deadline: " << this->Finished_before_deadline << "%\n";
	file << "Processsors: " << this->FCFS_size + this->RR_size + this->SJF_size+this->EDF_size << " [ " << this->FCFS_size << " FCFS, " << this->SJF_size << " SJF, " << this->EDF_size << " EDF, " << this->RR_size << " RR ]\n";
	file << " ******** Processors Load *********\n";
	for (int i = 0; i < this->FCFS_size + this->RR_size + this->SJF_size + this->EDF_size; i++)
	{
		if (i < this->FCFS_size)
		{
			file << "p" << i + 1 << "= " << FCFS_processor[i]->ProcessorLoad(avgtrt) << "%,\t";
			file << endl;
		}
		else if (i < this->FCFS_size + this->RR_size)
		{
			file << "p" << i + 1 << "= " << RR_processor[i - FCFS_size]->ProcessorLoad(avgtrt) << "%,\t";
			file << endl;
		}
		else if (i < this->FCFS_size + this->RR_size + this->SJF_size)
		{
			file << "p" << i + 1  << "= " << SJF_processor[i - FCFS_size - RR_size]->ProcessorLoad(avgtrt) << "%,\t";
			file << endl;
		}
		else
		{
			file << "p" << i + 1 << "= " << EDF_processor[i - FCFS_size - RR_size - SJF_size]->ProcessorLoad(avgtrt) << "%,\t";
			file << endl;
		}
	}
	file << "\n ********* Processors Utiliz ********** \n";
	for (int i = 0; i < this->FCFS_size + this->RR_size + this->SJF_size +this->EDF_size; i++)
	{
		if (i < this->FCFS_size)
		{
			file << "p" << i + 1 << "= " << FCFS_processor[i]->ProcessorUtilization() << "%,\t";
			AvgUtilization += FCFS_processor[i]->ProcessorUtilization();
			file << endl;
		}
		else if (i < this->FCFS_size + this->RR_size)
		{
			file << "p" << i + 1  << "= " << RR_processor[i - FCFS_size]->ProcessorUtilization() << "%,\t";
			AvgUtilization += RR_processor[i - FCFS_size]->ProcessorUtilization();
			file << endl;
		}
		else if (i < this->FCFS_size + this->RR_size + this->SJF_size)
		{
			file << "p" << i + 1 << "= " << SJF_processor[i - FCFS_size - RR_size]->ProcessorUtilization() << "%,\t";
			AvgUtilization += SJF_processor[i - FCFS_size - RR_size]->ProcessorUtilization();
			file << endl;
		}
		else 
		{
			file << "p" << i + 1 << "= " <<EDF_processor[i - FCFS_size - RR_size-SJF_size]->ProcessorUtilization() << "%,\t";
			AvgUtilization += EDF_processor[i - FCFS_size - RR_size -SJF_size]->ProcessorUtilization();
			file << endl;
		}
	}
	AvgUtilization = ((this->FCFS_size + this->RR_size + this->SJF_size + this->EDF_size) != 0)
		? AvgUtilization / (this->FCFS_size + this->RR_size + this->SJF_size + this->EDF_size) : 0;
	file << "\nAvgUtilization = " << AvgUtilization <<"%";
	file.close();
}


void scheduler::simulator(string file,string outFile)
{
	//Read File
	std::srand(time(0));
	if (!this->fileRead(file))
		return;
	MinPriority_Queue* runs;

	int Looper = 0; // to loop on processors
	Process* MovedProcess = nullptr;
	Process* RUNptr = nullptr;
	Process* BLKptr = nullptr;
	Process* randptr = nullptr;
	Processor* migptr1 = nullptr;
	Processor* migptr2 = nullptr;

	UI->PrintMode();
	int runsize = 0;
	while (this->TRMsize < this->no_of_processes && (this->FCFS_size+this->SJF_size+this->RR_size+this->EDF_size))
	{
		this->Time_step++;
		runsize = 0;
		runs = new MinPriority_Queue;


		this->WAITtoPRO();
		this->NewToshortest();
		this->SolveOverHeating();
		this->RunToBlk();
		this->BlkTShortest();
		this->KillingSignal();
		this->ProcessMigrationSimulator();
		this->LoopingProcessors(*runs,runsize);
		this->workStealing();
		UI->Printer(this->Time_step, FCFS_processor, RR_processor, SJF_processor, EDF_processor,
			this->FCFS_size, this->RR_size, this->SJF_size, this->EDF_size,this->BLK, this->BLKsize, this->TRM, this->TRMsize,
			*runs, runsize,this->WAIT_,this->WAIT_size,this->WAIT_RR,this->WAITRR_size,this->WAIT_SJF,this->WAITSJF_size);
		delete runs;

	}
	UI->End();



	this->fileWrite(outFile);
}

void scheduler::workStealing()
{
	if (this->Time_step % STL == 0 && this->shortest())
	{
		Processor* l_ongest = this->longest();
		Processor* s_hortest = this->shortest();
		int StealLimit = (float(l_ongest->getFinishTime() - s_hortest->getFinishTime()) / l_ongest->getFinishTime()) * 100;
		while (StealLimit > 40)
		{
			Process* stolen;
			l_ongest->PeekProcess(stolen);
			if (stolen)
			{
		//		if (stolen->isChild()) return;
				if (stolen->isChild())
				{
					int index = 0;
					dynamic_cast<FCFS_Processor*>(l_ongest)->stealNotForked(index, stolen);
				}
				else
					l_ongest->removePeek(stolen);
				if (stolen)
				{
					s_hortest->AddProcess(stolen);
					WorkSteal++;
				}
			}
			if (l_ongest->getFinishTime() == 0) {
				StealLimit = 0;
			}
			else {
				StealLimit = ((float)(l_ongest->getFinishTime() - s_hortest->getFinishTime()) / l_ongest->getFinishTime()) * 100;
			}
			if (!stolen)
				break;

		}
	}
}





Processor* scheduler::shortest()
{
	Processor* minRR = this->shortestRR();
	Processor* minFCFS = this->shortestFCFS();
	Processor* minSJF = this->shortestSJF();
	Processor* minEDF = this->shortestEDF();

	minRR = minRR ? minRR : (minFCFS ? minFCFS : (minSJF ? minSJF : minEDF));
	minFCFS = minFCFS ? minFCFS : (minRR ? minRR : (minSJF ? minSJF : minEDF));
	minSJF = minSJF ? minSJF : (minFCFS ? minFCFS : (minRR ? minRR : minEDF));
	minEDF = minEDF ? minEDF : (minFCFS ? minFCFS : (minRR ? minRR : minSJF));
	if (minRR == minFCFS && minFCFS == minSJF && minSJF == minEDF && minEDF == nullptr)
		return nullptr;
	Processor* min = (minFCFS->getFinishTime() <= minRR->getFinishTime()
		&& minFCFS->getFinishTime() <= minSJF->getFinishTime() 
		&& minFCFS->getFinishTime() <= minEDF->getFinishTime()) ?
		minFCFS : 
		(minRR->getFinishTime() <= minSJF->getFinishTime() 
			&& minRR->getFinishTime() <= minEDF->getFinishTime() ? minRR
			:(minSJF->getFinishTime() <= minEDF->getFinishTime() ? minSJF : minEDF));
	return min;

}

Processor* scheduler::shortestRR()
{
	if (this->RR_size == 0)
		return nullptr;
	Processor* min = nullptr;
	for (int i = 0; i < this->RR_size; i++) {
		if (!this->RR_processor[i]->isOverHeat()) {
			min = this->RR_processor[i];
			break;
		}
	}
	for (int i = 1; i < this->RR_size; i++) {
		if (min != nullptr && this->RR_processor[i]->getFinishTime() < min->getFinishTime() && !this->RR_processor[i]->isOverHeat()) {
			min = this->RR_processor[i];
		}
	}
	return min;
}

Processor* scheduler::shortestFCFS()
{
	if (this-FCFS_size == 0)
		return nullptr;
	Processor* min = nullptr;
	for (int i = 0; i < this->FCFS_size; i++) {
		if (!this->FCFS_processor[i]->isOverHeat()) {
			min = this->FCFS_processor[i];
			break;
		}
	}
	for (int i = 1; i < this->FCFS_size; i++) {
		if (min != nullptr && this->FCFS_processor[i]->getFinishTime() < min->getFinishTime() && !this->FCFS_processor[i]->isOverHeat()) {
			min = this->FCFS_processor[i];
		}
	}
	return min;
}

Processor* scheduler::shortestSJF()
{
	if (this->SJF_size == 0)
		return nullptr;
	Processor* min = nullptr;
	for (int i = 0; i < this->SJF_size; i++) {
		if (!this->SJF_processor[i]->isOverHeat()) {
			min = this->SJF_processor[i];
			break;
		}
	}
	for (int i = 1; i < this->SJF_size; i++) {
		if (min != nullptr && this->SJF_processor[i]->getFinishTime() < min->getFinishTime() && !this->SJF_processor[i]->isOverHeat()) {
			min = this->SJF_processor[i];
		}
	}
	return min;
}
Processor* scheduler::shortestEDF()
{
	if (this->EDF_size == 0)
		return nullptr;
	Processor* min = nullptr;
	for (int i = 0; i < this->EDF_size; i++) {
		if (!this->EDF_processor[i]->isOverHeat()) {
			min = this->EDF_processor[i];
			break;
		}
	}
	for (int i = 1; i < this->EDF_size; i++) {
		if (min != nullptr && this->EDF_processor[i]->getFinishTime() < min->getFinishTime() && !this->EDF_processor[i]->isOverHeat()) {
			min = this->EDF_processor[i];
		}
	}
	return min;
}

Processor* scheduler::longest()
{
	Processor* maxRR = this->longestRR();
	Processor* maxFCFS = this->longestFCFS();
	Processor* maxSJF = this->longestSJF();
	Processor* maxEDF = this->longestEDF();
	maxRR = maxRR ? maxRR : (maxFCFS ? maxFCFS : (maxSJF ? maxSJF : maxEDF));
	maxFCFS = maxFCFS ? maxFCFS : (maxRR ? maxRR : (maxSJF ? maxSJF : maxEDF));
	maxSJF = maxSJF ? maxSJF : (maxFCFS ? maxFCFS : (maxRR ? maxRR : maxEDF));
	maxEDF = maxEDF ? maxEDF : (maxFCFS ? maxFCFS : (maxRR ? maxRR : maxSJF));


	Processor* max = (maxFCFS->getFinishTime() > maxRR->getFinishTime()
		&& maxFCFS->getFinishTime() > maxSJF->getFinishTime()
		&& maxFCFS->getFinishTime() > maxEDF->getFinishTime()) ?
		maxFCFS :
		(maxRR->getFinishTime() > maxSJF->getFinishTime()
		&& maxRR->getFinishTime() > maxEDF->getFinishTime() ? maxRR
	: (maxSJF->getFinishTime() > maxEDF->getFinishTime() ? maxSJF : maxEDF));
	return max;
}

Processor* scheduler::longestRR()
{
	if (this->RR_size == 0)
		return nullptr;
	Processor* max = this->RR_processor[0];
	for (int i = 1; i < this->RR_size; i++) {
		if (this->RR_processor[i]->getFinishTime() > max->getFinishTime()) {
			max = this->RR_processor[i];
		}
	}
	return max;
}

Processor* scheduler::longestFCFS()
{
	if (this->FCFS_size == 0)
		return nullptr;
	Processor* max = this->FCFS_processor[0];
	for (int i = 1; i < this->FCFS_size; i++) {
		if (this->FCFS_processor[i]->getFinishTime() > max->getFinishTime()) {
			max = this->FCFS_processor[i];
		}
	}
	return max;
}

Processor* scheduler::longestSJF()
{
	if (this->SJF_size == 0)
		return nullptr;
	Processor* max = this->SJF_processor[0];
	for (int i = 1; i < this->SJF_size; i++) {
		if (this->SJF_processor[i]->getFinishTime() > max->getFinishTime()) {
			max = this->SJF_processor[i];
		}
	}
	return max;
}
Processor* scheduler::longestEDF() {
	if (this->EDF_size == 0)
		return nullptr;
	Processor* max = this->EDF_processor[0];
	for (int i = 1; i < this->EDF_size; i++) {
		if (this->EDF_processor[i]->getFinishTime() > max->getFinishTime()) {
			max = this->EDF_processor[i];
		}
	}
	return max;
}


bool scheduler::KillProcess(int ID)
{
	for (int i = 1; i < this->FCFS_size; i++)
	{
		Process* ptr = nullptr;
		dynamic_cast<FCFS_Processor*>(this->FCFS_processor[i])->Kill_FCFS_Process(ID, ptr);
		if (ptr)
		{
			ptr->Terminated(this->Time_step);
			this->To_TRM(ptr);
			this->deleteForked(ptr->getLeftFork());
			this->deleteForked(ptr->getRightFork());
			this->KilledProcess++;
			return true;
		}
	}
	return false;
}

void scheduler::WAITtoPRO()
{
	Processor* migptr;
	migptr = shortest();
	Process* ptr;
	while (migptr && WAIT_.dequeue(ptr)) {
		migptr->AddProcess(ptr);
		this->WAIT_size--;
		migptr = shortest();
	}




	migptr = shortestSJF();
	while (migptr && WAIT_SJF.dequeue(ptr)) {
		migptr->AddProcess(ptr);
		this->WAITSJF_size--;
		migptr = shortestSJF();
	}



	migptr = shortestRR();

	while (migptr && WAIT_RR.dequeue(ptr)) {
		migptr->AddProcess(ptr);
		this->WAITRR_size--;
		migptr = shortestRR();
	}
}

void scheduler::NewToshortest()
{
	Process* MovedProcess;
	while (this->NEWsize && NEW.peekFront()->getAT() == this->Time_step)
	{
		Processor* shortest_Processor = this->shortest();

		this->NEW.dequeue(MovedProcess);
		this->NEWsize--;
		if (shortest_Processor) {
			shortest_Processor->AddProcess(MovedProcess);
		}
		else {
			this->WAIT_size++;
			WAIT_.enqueue(MovedProcess);
		}
		MovedProcess = nullptr;

	}
}

void scheduler::RunToBlk()
{
	for (int j = 0; j < this->FCFS_size + this->RR_size + this->SJF_size + this->EDF_size; j++) {
		Process* ptr_BLK = nullptr;
		if (j < this->FCFS_size)
		{
			if (FCFS_processor[j]->BLKstep(ptr_BLK)) { //return int so you need many if conditions
				this->To_BLK(ptr_BLK);
				ptr_BLK = nullptr;
			}
		}
		else if (j < this->FCFS_size + this->RR_size) {

			if (this->RR_processor[j - this->FCFS_size]->BLKstep(ptr_BLK)) {
				this->To_BLK(ptr_BLK);
				ptr_BLK = nullptr;
			}
		}
		else if (j < this->FCFS_size + this->RR_size + this->SJF_size) {

			if (SJF_processor[j - this->FCFS_size - this->RR_size]->BLKstep(ptr_BLK)) {
				this->To_BLK(ptr_BLK);
				ptr_BLK = nullptr;
			}
		}
		else if (j < this->FCFS_size + this->RR_size + this->SJF_size + this->EDF_size) {

			if (EDF_processor[j - this->FCFS_size - this->RR_size - this->SJF_size]->BLKstep(ptr_BLK)) {
				this->To_BLK(ptr_BLK);
				ptr_BLK = nullptr;
			}
		}
	}

}

void scheduler::BlkTShortest()
{
	if (this->BLKsize && this->BLK.peekFront()->getTimeIODuration() == this->BLK_counter) {
		Process* Mover;
		this->BLK.dequeue(Mover);
		this->BLKsize--;
		Processor* shortest_Pro = this->shortest();
		if (shortest_Pro) {
			shortest_Pro->AddProcess(Mover);
		}
		else {
			this->WAIT_size++;
			WAIT_.enqueue(Mover);
		}
		this->BLK_counter = 0;
		Mover = nullptr;
	}
	else if (this->BLKsize) {
		this->BLK_counter++;
	}
	else {
		this->BLK_counter = 0;
	}
}

void scheduler::ProcessMigrationSimulator()
{
	Processor* migptr1;
	Processor* migptr2;
	for (int i = 0; i < this->FCFS_size + this->RR_size; i++) {
		//for FCFS
		Process* ptr;
		if (i < this->FCFS_size)
		{
			while (dynamic_cast<FCFS_Processor*>(FCFS_processor[i])->ProcessMigration(ptr, RR_size, Time_step))
			{
				migptr1 = this->shortestRR();
				this->Migration_MaxW++;
				if (migptr1) {
					migptr1->AddProcess(ptr);
				}
				else if(this->RR_size){
					this->WAITRR_size++;
					WAIT_RR.enqueue(ptr);
				}
				else {
					this->WAIT_size++;
					WAIT_.enqueue(ptr);
				}
				ptr = nullptr;
			}
		}
		//for RR
		else if (i < this->FCFS_size + this->RR_size)
		{
			while (dynamic_cast<RR_Processor*>(RR_processor[i - this->FCFS_size])->ProcessMigration(ptr, SJF_size))
			{
				migptr2 = shortestSJF();
				if (migptr2) {
					migptr2->AddProcess(ptr);
				}
				else if(this->SJF_size){
					this->WAITSJF_size++;
					WAIT_SJF.enqueue(ptr);
				}
				else {
					this->WAIT_size++;
					WAIT_.enqueue(ptr);
				}
				ptr = nullptr;
				Migration_RTf++;
			}
		}

	}
}

void scheduler::LoopingProcessors(MinPriority_Queue& runs, int& runsize)
{
	for (int i = 0; i < this->FCFS_size + this->RR_size + this->SJF_size + this->EDF_size; i++) {


		//for FCFS
		Process* ptr;
		if (i < this->FCFS_size)
		{
			int x = this->FCFS_processor[i]->SchedulingAlgorithm(ptr, this->Time_step); //return int so you need many if conditions
			switch (x) {
			case 0: //in RUN
				runs.add(i, ptr);
				if (ptr)
					runsize++;
				break;
			case 2://TRM

				ptr->Terminated(this->Time_step);
				this->TermenateForked(ptr->getLeftFork());
				this->TermenateForked(ptr->getRightFork());
				this->To_TRM(ptr);
				ptr = nullptr;
				break;
			case 3://FORKING

				if (this->FCFS_size && this->shortestFCFS()) {
					Processor* short_one = this->shortestFCFS();
					Process* newForked = new Process(true);
					newForked->Read(++this->no_of_processes, this->Time_step, ptr->getTimeDone());
					newForked->setParent(ptr);
					ptr->setFork(newForked);
					this->ForkedProcess++;
					short_one->AddProcess(newForked);
				}
				break;
				//complete
			default:
				break;
			}
		}
		//for RR
		else if (i < this->FCFS_size + this->RR_size)
		{
			int x = this->RR_processor[i - this->FCFS_size]->SchedulingAlgorithm(ptr, this->Time_step); //return int so you need many if conditions
			switch (x) {
			case 0: //in RUN
				runs.add(i, ptr);
				if (ptr)
					runsize++;
				break;
			case 2://TRM
;
				ptr->Terminated(this->Time_step);
				this->TermenateForked(ptr->getLeftFork());
				this->TermenateForked(ptr->getRightFork());
				this->To_TRM(ptr);
				ptr = nullptr;
				break;
				//complete
			default:break;
			}
		}
		//for SJF
		else if (i < this->FCFS_size + this->RR_size + this->SJF_size)
		{
			int x = this->SJF_processor[i - FCFS_size - this->RR_size]->SchedulingAlgorithm(ptr, this->Time_step); //return int so you need many if conditions
			switch (x) {
			case 0: //in RUN
				runs.add(i, ptr);
				if (ptr)
					runsize++;
				break;
			case 2://TRM

				ptr->Terminated(this->Time_step);
				this->TermenateForked(ptr->getLeftFork());
				this->TermenateForked(ptr->getRightFork());
				this->To_TRM(ptr);
				ptr = nullptr;
				break;
				//complete
			default:break;
			}
		}
		else if (i < this->FCFS_size + this->RR_size + this->SJF_size + this->EDF_size)
		{
			int x = this->EDF_processor[i - FCFS_size - this->RR_size - this->SJF_size]->SchedulingAlgorithm(ptr, this->Time_step); //return int so you need many if conditions
			switch (x) {
			case 0: //in RUN
				runs.add(i, ptr);
				if (ptr)
					runsize++;
				break;
			case 2://TRM

				ptr->Terminated(this->Time_step);
				this->TermenateForked(ptr->getLeftFork());
				this->TermenateForked(ptr->getRightFork());
				this->To_TRM(ptr);
				ptr = nullptr;
				break;
				//complete
			default:break;
			}
		}
	}

}

void scheduler::SolveOverHeating()
{
	for (int i = 0; i < this->FCFS_size + this->RR_size + this->SJF_size + this->EDF_size; i++) {
		Process* ptr;
		if (i < this->FCFS_size)
		{
			if (this->FCFS_processor[i]->isOverHeat()) {
				while (this->FCFS_processor[i]->RemoveRun(ptr)) {
					if (ptr->isChild()) {
						ptr->getParent()->removeChild(ptr);
						ptr->Terminated(this->Time_step);
						this->To_TRM(ptr);
					}
					else {
						if (this->shortest())
							this->shortest()->AddProcess(ptr);
						else {
							this->WAIT_size++;
							WAIT_.enqueue(ptr);
						}
					}
				}
			}
		}
		//for RR
		else if (i < this->FCFS_size + this->RR_size)
		{
			if (this->RR_processor[i - this->FCFS_size]->isOverHeat()) {
				while (this->RR_processor[i - this->FCFS_size]->RemoveRun(ptr)) {
					if(this->shortest())
						this->shortest()->AddProcess(ptr);
					else {
						this->WAIT_size++;
						WAIT_.enqueue(ptr);
					}
				}
			}
		}
		//for SJF
		else if (i < this->FCFS_size + this->RR_size + this->SJF_size)
		{
			if (this->SJF_processor[i - FCFS_size - this->RR_size]->isOverHeat()) {
				while (this->SJF_processor[i - FCFS_size - this->RR_size]->RemoveRun(ptr)) {
					if (this->shortest())
						this->shortest()->AddProcess(ptr);
					else {
						this->WAIT_size++;
						WAIT_.enqueue(ptr);
					}
				}
			}
		}
		else if (i < this->FCFS_size + this->RR_size + this->SJF_size + this->EDF_size)
		{
			if (this->EDF_processor[i - FCFS_size - this->RR_size - this->SJF_size]->isOverHeat()) {
				while (this->EDF_processor[i - FCFS_size - this->RR_size - this->SJF_size]->RemoveRun(ptr)) {
					if (this->shortest())
						this->shortest()->AddProcess(ptr);
					else {
						this->WAIT_size++;
						WAIT_.enqueue(ptr);
					}
				}
			}
		}
	}
}

void scheduler::KillingSignal()
{
	int tk = this->Sigkill_Time.peekFront();
	while (this->Time_step == tk)
	{
		int id;
		Sigkill_PID.dequeue(id);
		this->KillProcess(id);
		this->Sigkill_Time.dequeue(tk);
		tk = this->Sigkill_Time.peekFront();
	}

}



void scheduler::deleteForked(Process* Fork)
{
	// if ptr==nullptr 
	if (!Fork)
		return;


	for (int i = 0; i < this->FCFS_size; i++)
	{
		Process* ptr = nullptr;
		dynamic_cast<FCFS_Processor*>(this->FCFS_processor[i])->Kill_FCFS_Process(Fork->getPID(), ptr);
		if (ptr)
		{
			ptr->Terminated(this->Time_step);
			this->To_TRM(ptr);
			this->KilledProcess++;
			break;
		}
	}

	deleteForked(Fork->getLeftFork());
	deleteForked(Fork->getRightFork());

}

void scheduler::TermenateForked(Process* Fork)
{
	if (!Fork)
		return;




	TermenateForked(Fork->getLeftFork());
	TermenateForked(Fork->getRightFork());
	for (int i = 0; i < this->FCFS_size; i++)
	{
		Process* ptr = nullptr;
		if (dynamic_cast<FCFS_Processor*>(this->FCFS_processor[i])->Kill_FCFS_Process(Fork->getPID(), ptr))
		{
			ptr->Terminated(this->Time_step);
			this->To_TRM(ptr);
			return;
		}
	}
}

scheduler::~scheduler()//need someone to see it
{
	delete UI;

	for (int i = 0; i < this->RR_size; i++)
		delete this->RR_processor[i];
	for (int i = 0; i < this->SJF_size; i++)
		delete this->SJF_processor[i];
	for (int i = 0; i < this->FCFS_size; i++)
		delete this->FCFS_processor[i];
	delete[this->RR_size]RR_processor;
	delete[this->SJF_size]SJF_processor;
	delete[this->FCFS_size]FCFS_processor;
}
void scheduler::To_BLK(Process*& scheduldedProcess)
{
	BLK.enqueue(scheduldedProcess);
	this->BLKsize++;
}
void scheduler::To_TRM(Process*& scheduldedProcess)
{
	if (scheduldedProcess->getDeadline() < Time_step)
		Finished_before_deadline++;
	TRM.enqueue(scheduldedProcess);
	this->TRMsize++;
}