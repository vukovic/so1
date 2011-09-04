//File: KrnlSem.h

#ifndef _KRNLSEM_H_
#define _KRNLSEM_H_

#include <QUEUE.H>
#include <asm.h>

typedef unsigned int Time;

class PCB;


class KernelSem{
private:
	Queue Qblocked;
	static KrnlSemQueue allSems;
	int  value;
public:
	int block(Time	timeToWait);
	void deblock();
	
	int decVal(){return --value;}
	int incVal(){return value++;}
	int getVal()const;
	void init(int i );
	
	KernelSem(int i);
	~KernelSem();
	
	static void tick(){allSems.tick();}
	
	void tick();
	void wake(PCB* pcbToWake, int byTimer);
};
#endif