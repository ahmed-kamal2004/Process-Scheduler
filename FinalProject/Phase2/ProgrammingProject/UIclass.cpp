#include "UIclass.h"
#define sleepingTime 1000
void UIclass::GetMode()
{
	int Output;
	cout << "1.Interactive Mode\n2.Step Mode\n3.Silent Mode\t\t\tType Mode Num you prefer !" << endl;
	cin >> Output;
	this->Mode=Output;
}
void UIclass::Printer(int Time, Processor**FCFSlista, Processor**RRlista, Processor**SJFlista, Processor** EDFlista, int FCFSsize,
//void UIclass::Printer(int Time, Processor* FCFSlista, Processor* RRlista, Processor* SJFlista, int FCFSsize,
	int RRsize, int SJFsize,int EDFsize, Queue<Process*>&BLKlista, int BLKsize, Queue<Process*>&TRMlista, int TRMsize, MinPriority_Queue&Run,int size, Queue<Process*>& WAIT, int waitSize, Queue<Process*>& WAITRR, int waitRRSize, Queue<Process*>& WAITSJF, int waitSJFSize)
{

	if (this->Mode == 1 || this->Mode == 2 ) {
		this->PrintTIME(Time);
		int Index = 0;
		for (int i = 0; i < FCFSsize + RRsize + SJFsize + EDFsize; i++) {
			if (i < FCFSsize) {
				cout << "Processor " << ++Index << " ";
				FCFSlista[i]->print();
			}
			else if (i < FCFSsize + RRsize) {
				cout << "Processor " << ++Index << " ";
				RRlista[i - FCFSsize]->print();
			}
			else if (i < FCFSsize + RRsize + SJFsize) {
				cout << "Processor " << ++Index << " ";
				SJFlista[i - FCFSsize - RRsize]->print();
			}
			else {
				cout << "Processor " << ++Index << " ";
				EDFlista[i - FCFSsize - RRsize - SJFsize]->print();
			}
		}
		this->RUN_STATE();
		this->PrintRUN(Run,size);
		this->BLK_STATE();
		this->PrintBLK(BLKlista, BLKsize);

		this->WAIT_STATE();
		this->PrintWAIT(WAIT,waitSize);
		this->WAITRR_STATE();
		this->PrintWAITRR(WAITRR,waitRRSize);
		this->WAITSJF_STATE();
		this->PrintWAITSJF(WAITSJF,waitSJFSize);
		this->TRM_STATE();
		this->PrintTRM(TRMlista, TRMsize);
	}
	this->WAIT();
}
void UIclass::PrintMode()
{
	cout << "##################################################################" << endl;
	if (this->Mode == 1)
		cout << "Interactive Mode ,Simulation starts" << endl;
	else if (this->Mode == 2)
		cout << "Step By Step Mode ,Simulation starts" << endl;
	else if (this->Mode == 3)
		cout << "Silent Mode ,Simulation starts" << endl;
	else
		cout << "You Entered Wrong Input for Mode,Sorry" << endl;
	cout << "##################################################################" << endl;

}

void UIclass::PrintTIME(int& Time)
{

	if (this->Mode == 1 || this->Mode == 2) {
		cout << "######################################################################################################################################" << endl;
		cout << "Current TimeStep: " << Time << endl;
	}
	else
		return;
}
void UIclass::RDY_STATE()
{
	cout << "--------------------- RDY Process ---------------------" << endl;
}

void UIclass::BLK_STATE()
{
	cout << "--------------------- BLK Process ---------------------" << endl;
}
void UIclass::PrintBLK(Queue<Process*>&BLKLIST, int size)
{
	cout << size << " BLK : ";
	BLKLIST.print();
	cout << endl;
}
void UIclass::RUN_STATE()
{
	cout << "--------------------- RUN Process ---------------------" << endl;
}
void UIclass::PrintRUN(MinPriority_Queue&q,int size)
{
	
	Process* ptr = nullptr;
	cout <<size<<" RUN : ";
	while (!q.isEmpty())
	{
		ptr = q.peek().getContent();
		
		if (ptr!=nullptr)
		{
			cout << (ptr)->getPID() << "(P"<<q.peek().getPriority()+1<<") ,";
		}
		q.remove();
	}
	cout << endl;
}
void UIclass::WAIT_STATE()
{
	cout << "--------------------- WAIT Process ---------------------" << endl;
}
void UIclass::PrintWAIT(Queue<Process*>& WAIT, int size)
{
	cout << size << " WAIT ALL : ";
	WAIT.print();
	cout << endl;
}
void UIclass::WAITRR_STATE()
{
	cout << "--------------------- WAIT RR Process ---------------------" << endl;
}
void UIclass::PrintWAITRR(Queue<Process*>&WAIT, int size)
{
	cout << size << " WAIT RR : ";
	WAIT.print();
	cout << endl;
}
void UIclass::WAITSJF_STATE()
{
	cout << "--------------------- WAIT SJF Process ---------------------" << endl;

}
void UIclass::PrintWAITSJF(Queue<Process*>&WAIT, int size)
{
	cout << size << " WAIT SJF : ";
	WAIT.print();
	cout << endl;
}
void UIclass::TRM_STATE()
{
	cout << "--------------------- TRM ---------------------" << endl;
}
void UIclass::PrintTRM(Queue<Process*>& TRMLIST, int size)
{
	cout << size << " TRM : ";
	TRMLIST.print();
	cout << endl;
}
void UIclass::WAIT()
{
	if (this->Mode == 1)
		system("pause");
	else if (this->Mode == 2)
		Sleep(sleepingTime);
	else if (this->Mode == 3)
		return;
}
void UIclass::End()
{
	cout << " *** Simulation ends, Output File created ***\nThanks For using our Program :)." << endl;
}