//File: Thread.cpp

#include <THREAD.H>
#include <SCHEDULE.H>
#include <PCB.H>
#include <KRNLSEM.H>
#include <MYLOCK.H>


void Thread::start(){
	myPCB->status = ready;
	lock();
	Scheduler::put(myPCB);
	unlock();
}
int Thread::waitToComplete(){
	lock();
	if(!myPCB->finished){
		myPCB->numBlockedTillFinish++;
		myPCB->blockedTillFinish.wait(0);
	}
	unlock();
	return myPCB->finished;
}
int Thread::wakeUp(){
	lock();
	KernelSem* ret=0;
	if((ret = myPCB->blockedOn)!=0){
		myPCB->blockedOn->wake(myPCB,0); // arg 0 znaci da nije od timera
	}
	unlock();
	//dispatch();//proveriti da li je potreban dispatch()
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
	lock();
	myPCB = new PCB(name, stackSize, timeSlice, this);
	unlock();
}

/*-------------------------------------------------------------------------------
	Ako je status ready_wokenUp znaci da je nit odblokirana na nekom semaforu
	nasilno, sto znaci da treba da se izadje iz waitForChildren. U tom slucaju 
	mozda treba smanjiti broj djece na 0? 
*/
int Thread::waitForChildren(){
	lock();
	while(myPCB->numChildren>0){
		myPCB->waitChildren.wait(0);
		if(myPCB->status == ready_wokenUp){
			unlock();
			return 0;
		}
		myPCB->numChildren--;
	}
	unlock();
	return 1;
}
//--------------------------------------------------------------------------------