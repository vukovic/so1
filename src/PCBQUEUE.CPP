//File: PCBQueue.cpp

#include <PCBQUEUE.H>
#include <PCB.H>
#include <MYLOCK.H>
#include <KRNLSEM.H>
#include <string.h>

PCBQueue& PCBQueue::addLast(PCB* newPCB){
	lock();
	Elem* e = new Elem(newPCB);
	if (!first){
		first=e;
		e->next=e->prev=0;
	}else{
		last->next=e;
		e->prev=last;
		e->next=0;
	}
	last = e;
	unlock();
	return *this;
}
PCB* PCBQueue::getPCBbyId(ID id){
	Elem* cur = first;
	for(; cur && cur->pcb->myID!=id; cur=cur->next);
	return cur->pcb;
}
PCB* PCBQueue::getPCBbyName(TName name){
	Elem* cur=first;
	for(; cur && strcmp(cur->pcb->myName,name);cur=cur->next);
	return cur->pcb;
}
PCBQueue& PCBQueue::remove(PCB* pcbToRemove){
	Elem* cur=first;
	for(; cur && cur->pcb!=pcbToRemove;cur=cur->next);
	if (first && cur == first ) first = first->next;
	else if(last && cur==last)last = last->prev;
	if(cur->prev)cur->prev->next=cur->next;
	if(cur->next)cur->next->prev=cur->prev;
	delete cur;
	return *this;
}

PCBQueue::Elem::Elem(PCB* newPCB){
	pcb=newPCB;
}