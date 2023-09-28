#include"Process.h"


Process::Process(bool isChild )
{
	Forked = false;
	Child = isChild;
	Parent = nullptr;
	LFork = RFork = nullptr;
	IODtotal = 0;
	this->state_ = NEW;
	this->index = this->TimeDone = this->PID = this->AT = this->RT = this->CT
		= this->TimeDone = this->TT = this->TRT = this->WT = this->N =this->TimeIODuration = 0;
}

void Process::Read(int pid, int at, int ct, int n, int* ior, int* iod,int dead)
{
	this->AT = at;
	this->CT = this->TimeDone = ct;
	this->PID = pid;
	this->N = n;
	this->IO_D = new int[this->N];
	this->IO_R = new int[this->N];

	for (int i = 0; i < this->N; i++) {
		this->IO_D[i] = iod[i];
		this->IO_R[i] = ior[i];
	}

	this->DeadLine = dead;
}

void Process::setResponsetime(int CPU_FIRST_TIME)
{
	this->RT = CPU_FIRST_TIME - this->AT;

}
int Process::getResponsetime()
{
	return this->RT;
}

//untouchable
bool Process::Step()
{
	if (this->TimeDone)
	{
		this->TimeDone--;
		return true;
	}
	return false;
}
//untouchable
int Process::getAT()
{
	return this->AT;
}

int Process::getPID()
{
	return this->PID;
}

int Process::getRT()
{
	return RT;
}

int Process::getCT()
{
	return this->CT;
}

int Process::getTT()
{
	return this->TT;
}

int Process::_TRT_(int time)
{
	if (this->state_ != TRM)
	{
		TRT=time - AT;
	}
		return getTRT();
}
int Process::getTRT()
{
	return TRT;
}
int Process::getWT(int time)
{
	if (this->state_ != TRM)
		return _TRT_(time) - getCT() + getTimeDone();
	else
		return WT;
}

int Process::getN()
{
	return N;
}
int Process::GetTotalIOD()
{
	return this->IODtotal;
}
int Process::getDeadline()
{
	return this->DeadLine;
}
void Process::setDeadline(int dead)
{
	this->DeadLine = dead;
}
bool Process::isChild()
{
	return this->Child;
}
bool Process::isForked()
{
	return this->LFork || this->RFork;
}
bool Process::isTOTForked()
{
	return this->LFork && this->RFork;
}
Process* Process::getLeftFork()
{
	return this->LFork;
}
Process* Process::getRightFork()
{
	return this->RFork;
}
Process* Process::getParent()
{
	return this->Parent;
}
void Process::removeChild(Process*&ptr)
{
	if (this->LFork == ptr)
		this->LFork = nullptr;
	else if (this->RFork == ptr)
		this->RFork = nullptr;
}
void Process::setParent(Process*ptr)
{
	this -> Parent = ptr;
}
bool Process::setFork(Process* child)//left first
{
	if(this->isTOTForked())
		return false;
	if (!this->LFork) {
		this->LFork = child;
	}
	else if (!this->RFork) {
		this->RFork = child;
	}
	return true;

	

}
int Process::getTimeIODuration()
{
	return this->TimeIODuration;
}

int Process::getTimeDone()
{
	return TimeDone;
}

//untouchable
bool Process::GetIO_R(int& IOR)
{
	IOR == 0;
	if (index >= N)
		return false;
	IOR = this->IO_R[this->index];
	return true;
}
bool Process::GetIO_D(int& IOD)
{
	IOD = 0;
	if (index >= N)//means that it cant have IO inputs
		return false;
	IOD = this->IO_D[this->index];
	return true;
}	//untouchable

void Process::UpdateIO()
{
	this->index++;
}

void Process::Terminated(int& T)
{
	this->setState(TRM);
	this->TT = T;
	this->TRT = this->TT - this->AT;
	this->WT = this->TRT - (this->CT - this->TimeDone);
	this->state_ = TRM;
}
void Process::Blocked()
{
	TimeIODuration = 0;
	this->setState(BLK);
	this->GetIO_D(this->TimeIODuration);
	IODtotal += this->TimeIODuration;
	this->UpdateIO();
}
void Process::print()
{
	cout << this->PID;
}

State Process::getState()
{
	return state_;
}
void Process::setState(State S)
{
	state_=S;
}
Process::~Process()
{
	if (this->N) {
		delete[N]IO_D;
		delete[N]IO_R;
	}
}
