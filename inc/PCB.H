//File: PCB.h
#ifndef _PCB_H_
#define _PCB_H_
#include <SEMAPHOR.H>
#include <PCBQUEUE.H>

typedef unsigned int Time;
typedef int ID;
typedef char* TName;
typedef unsigned long StackSize;

class Thread;

enum Status {ready=0, ready_wokenUp, blocked, runner};

class PCB{
public:
	Semaphore waitChildren;
	unsigned int numChildren;
	Semaphore blockedTillFinish;
	unsigned int numBlockedTillFinish;
	KernelSem* blockedOn;
	
	PCB* parent;
	TName myName;
	static ID ukId;
	ID myID;
	Thread* myThread;
	unsigned finished;
	unsigned int timeSlice;
	Status status;
	
	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned int *stack;
 
	PCB(TName, StackSize, Time, Thread*);
	~PCB();
	
	static void entryPoint(Thread*);
	static PCBQueue allPCBs;
	static PCB* getPCBbyId(ID);
	static PCB* getPCBbyName(TName);
};

#endif