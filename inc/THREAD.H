//File: Thread.h
#ifndef _THREAD_H_
#define _THREAD_H_


typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time;
const Time defaultTimeSlice = 2;
typedef int ID;
typedef char* TName;

class PCB;
void dispatch();

class Thread{
public:
	void start();
	int waitToComplete();
	int wakeUp();
	
	ID getId();
	TName getName();
	
	static Thread* getThreadById(ID id);
	static ID getIdOf(TName name);
	
	virtual ~Thread();
protected:
	friend class PCB;
	Thread(TName name, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	
	virtual void run(){}
	int waitForChildren();
private:
	PCB* myPCB;
};
#endif