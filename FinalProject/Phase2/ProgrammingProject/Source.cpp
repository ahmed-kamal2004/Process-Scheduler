#include"Heap_implementation.h"
#include"Priority_Queue.h"
#include"Queue.h"
#include"FCFS_Processor.h"
#include"RR_Processor.h"
#include"SJF_Processor.h"
#include"Process.h"
#include"Processor.h"
#include"scheduler.h"
#include <filesystem>
#include <fstream>
#include<iostream>


int main() {
	scheduler* s;
	int counter = 2;
	s = new scheduler;
	s->simulator("IN"+ to_string(counter), "Out"+ to_string(counter));
	system("pause");
	delete s;


}
