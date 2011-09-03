//File: KrnlSem.cpp

#include <KRNLSEM.H>
#include <PCB.H>
#include <SCHEDULE.H>
#include <DISPATCH.H>
#include <LOCK.H>


KernelSem::KernelSem(int i):Qblocked(this){
	init(i);
}
void KernelSem::init(int i){
	lock();
	value = i;
	unlock();
}
int KernelSem::block(Time timeToWait){
	int ret = 0;
	running->blockedOn=this;
	lock();
	running->status=blocked;
	Qblocked.put((PCB*)running, timeToWait);
	dispatch();
	asm mov word ptr [bp-2], ax
	unlock();
	return ret;
}
void KernelSem::deblock(){
	lock();
	PCB* pcb = Qblocked.get();
	pcb->blockedOn=0;
	editAX(pcb,1);
	pcb->status=ready;
	Scheduler::put(pcb);
	unlock();
}
void KernelSem::wake(PCB* pcbToWake, int byTimer){
	lock();
	Qblocked.remove(pcbToWake,byTimer);
	unlock();
}
void KernelSem::tick(){
	lock();
	if(Qblocked.first && ++Qblocked.passed>=Qblocked.first->data->value && Qblocked.first->data->value)wake(Qblocked.first->data->pcb,1);
	unlock();
}
int KernelSem::getVal()const{
	return value;
}
KernelSem::~KernelSem(){
	//sta ovdje treba da se uradi???
}

