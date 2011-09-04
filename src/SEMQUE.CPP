//File: SemQue.cpp

#include <SEMQUE.H>
#include <KRNLSEM.H>
#include <INTLOCK.H>

KrnlSemQueue::Elem::Elem(KernelSem* newKs){
	ks=newKs;
}
KrnlSemQueue& KrnlSemQueue::addLast(KernelSem* newKs){
	lock();
	Elem *e = new Elem(newKs);
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
KrnlSemQueue& KrnlSemQueue::remove(KernelSem* krnlSemToRemove){
	lock();
	Elem* cur = first;
	for(; cur && cur->ks!=krnlSemToRemove;cur=cur->next);
	if(cur && cur->prev)cur->prev->next=cur->next;
	if(cur && cur->next)cur->next->prev=cur->prev;
    if(cur)	delete cur;
	unlock();
	return *this;
}
void KrnlSemQueue::tick(){
	lock();
	for(Elem* cur = first; cur ; cur=cur->next)cur->ks->tick();
	unlock();
}