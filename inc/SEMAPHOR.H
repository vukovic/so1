//File: Semaphor.h
#ifndef _SEMAPHOR_H_
#define _SEMAPHOR_H_
extern int semPreempt;
typedef unsigned int Time;

class KernelSem;

class Semaphore{
public:
	Semaphore(int init=1);
	~Semaphore();
	
	virtual int wait(Time maxTimeToWait);
	virtual void signal();
	
	int val() const;
private:
	KernelSem* myImpl;
};
#endif