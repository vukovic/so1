//File: Thread.cpp

#include <THREAD.H>
#include <SCHEDULE.H>
#include <PCB.H>
#include <KRNLSEM.H>
#include <LOCK.H>


void Thread::start(){
	myPCB->status = ready;
	Scheduler::put(myPCB);
}
int Thread::waitToComplete(){
	if(!myPCB->finished){
		myPCB->numBlockedTillFinish++;
		myPCB->blockedTillFinish.wait(0);
	}
	return myPCB->finished;
}
int Thread::wakeUp(){
	KernelSem* ret=0;
	if((ret = myPCB->blockedOn)!=0){
		myPCB->blockedOn->wake(myPCB,0); // arg 0 znaci da nije od timera
	}
	dispatch();//proveriti da li je potreban dispatch()
	return ret!=0;
}
ID Thread::getId(){
	return myPCB->myID;
}
TName Thread::getName(){
	return myPCB->myName;
}
Thread* Thread::getThreadById(ID id){
	PCB* pcb = PCB::getPCBbyId(id);
	if(pcb)return pcb->myThread;
	return 0;
}
ID Thread::getIdOf(TName name){
	PCB* pcb = PCB::getPCBbyName(name);
	if(pcb)return pcb->myID;
	return -1;
}
Thread::~Thread(){
	waitToComplete();
	delete myPCB;
}
Thread::Thread(TName name, StackSize stackSize,Time timeSlice){
	lock()
	myPCB = new PCB(name, stackSize, timeSlice, this);
	unlock()
}

/*-------------------------------------------------------------------------------
	Ako je status ready_wokenUp znaci da je nit odblokirana na nekom semaforu
	nasilno, sto znaci da treba da se izadje iz waitForChildren. U tom slucaju 
	mozda treba smanjiti broj djece na 0? 
*/
int Thread::waitForChildren(){
	while(myPCB->numChildren>0){
		myPCB->waitChildren.wait(0);
		if(myPCB->status == ready_wokenUp)return 0;
		myPCB->numChildren--;
	}
	return 1;
}
//--------------------------------------------------------------------------------