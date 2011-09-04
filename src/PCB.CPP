//File PCB.cpp

#include <PCB.H>
#include <dos.h>
#include <THREAD.H>
#include <DISPATCH.H>
#include <MYLOCK.H>


ID PCB::ukId=0;
PCBQueue PCB::allPCBs=PCBQueue();

PCB::PCB(TName name , StackSize stackSize, Time _timeSlice, Thread* _thread):waitChildren(0),blockedTillFinish(0){
	blockedOn=0;
	myID =ukId++;
	myName = (name==0)?""/*"thread+myID da li moze da se koristi string.h"*/:name;
	parent = (PCB*)running;
	if(parent)parent->numChildren++;
	myThread=_thread;
	timeSlice=_timeSlice;
	finished = 0;
	numChildren=0;
	numBlockedTillFinish=0;
	//provera da li stackSize zadovoljava uslov!!!!!!!
	lock();
	unsigned int* _stack = new unsigned int[stackSize];

	_stack[stackSize-1]=FP_SEG(_thread);
	_stack[stackSize-2]=FP_OFF(_thread);	
  /*_stack[stackSize-3]=PC_ret za entryPoint()*/
  /*_stack[stackSize-4]=PC_ret za entryPoint()*/
    _stack[stackSize-5]=0x200;//pswI
	_stack[stackSize-6]=FP_SEG(entryPoint);
	_stack[stackSize-7]=FP_OFF(entryPoint);
  /*_stack[stackSize-8..stackSize-15]=ax,bx,cx,dx,es,ds,si,di;*/
  /*_stack[stackSize-16]=pokazivac na stari BP;*/
    ss=FP_SEG(_stack+stackSize-16);
	sp=bp=FP_OFF(_stack+stackSize-16);
	
	stack=_stack;
	allPCBs.addLast(this);
	unlock();
}
PCB::~PCB(){
	lock();
	allPCBs.remove(this);
	delete [] stack;
	unlock();
}
void PCB::entryPoint(Thread* myThrd){
	myThrd->run();
	myThrd->myPCB->finished=1;
	if(myThrd->myPCB->parent)myThrd->myPCB->parent->waitChildren.signal();
	while(myThrd->myPCB->numBlockedTillFinish>0){
		myThrd->myPCB->blockedTillFinish.signal();
		myThrd->myPCB->numBlockedTillFinish--;
	}
	dispatch();
}
PCB* PCB::getPCBbyId(ID id){
	lock();
	PCB* pcb = allPCBs.getPCBbyId(id);
	unlock();
	return pcb;
} 
PCB* PCB::getPCBbyName(TName name){
	lock();
	PCB* pcb = allPCBs.getPCBbyName(name);
	unlock();
	return pcb;
}