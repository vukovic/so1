//File: Queue.cpp
#include <QUEUE.H>
#include <KRNLSEM.H>
#include <SCHEDULE.H>
#include <PCB.H>
#include <MYLOCK.H>



/*
	Ova f-ja dodaje pcb u red blokiranih odrzavajuci neopadajuci poredak po vremenu
	koje nit moze da provede blokirana, s tim sto se niti koje mogu neograniceno da 
	ostanu blokirane stavljaju na kraj reda.
*/


Queue& Queue::put(PCB* pcb, int timeToWait){ 
	lock();
	Elem* newElem = new Elem(pcb,timeToWait);
	if(!first || timeToWait==0){
		addLast(newElem);
	}else{
		if(first->prio)first->prio-=passed;//azurira glavu liste
		passed = 0;
		Elem* cur = first;
		for (; cur->next && cur->prio && cur->prio <= timeToWait
								&& cur->next->prio != 0; cur=cur->next);
		if (!cur->prio || (cur->prio > timeToWait))addBefore(newElem,cur);
		else if(!cur->next)addLast(newElem);
		else addAfter(cur,newElem);
	}
	unlock();
	return *this;
}

/*
vraca element koji moze najmanje da ceka.
*/

PCB* Queue::get(){
	lock();
	Elem* cur = first;
	if (first)first=first->next;
	if(first){
		first->prev=0;
		if (first->prio)first->prio-=passed;
	}else last = 0;
	passed=0;
	PCB* pcb = cur?cur->pcb:0;
	delete cur;
	unlock();
	return pcb;
}

/*	
	Ova funkcija se poziva od strane f-je wake iz KRNLSEM.CPP a sluzi za budjenje niti.
	Prvo se trazi zadati pcb u redu blokiranih, ako se nadje onda se 
	podesavaju parametri pcb-a(blockedOn se ponistava, odredjuje se povratna vrijednost
		f-je wait(int) na osn param byTimer, status se postavlja na ready_wokenUp,
		i vrijednost semafora, na kome je nit blokirana se osvjezva u skladu sa budjenjem te niti.),
	onda se vrsi prevezivanje elementa u redu i ispituje da li je ciljani element bio prvi element 
	u redu, ako jeste onda se pokazivac na prvog osvjezava. Na kraju se brise izbaceni element.
	
	Iteracija se ponavlja ako postoji jos elemenata, ako sledeci element nije element koji
	moze "beskonacno" da ceka na semaforu, ako je ciljani element bio prvi i uz to
	ako je proslo dovoljno vremena da se i on izbaci iz reda.
	
*/
void Queue::remove(PCB* pcb, int byTimer){
	lock();
	Elem* cur = first;
	for(;cur && cur->pcb !=pcb; cur=cur->next);
	if (cur && cur!=first){//ako nadjeni nije prvi	
		wake(cur,0);
		delete cur;
	}else if (cur && cur == first && !byTimer){ //ako je nadjeni prvi ali nije od timera, onda ne treba proveravati ostale
		wake(cur,0);
		delete cur;
		if(first->prio)first->prio-=passed;
		passed=0;
	}else if (cur && cur == first && byTimer){//ako je nadjeni prvi i jeste od timera onda treba proveravati ostale.
		do{
			wake (first,2);
			delete cur;
			cur = first;
		}while(first && first->prio && ((first->prio-=passed)<=0));
		passed=0;
	}
	unlock();
}
void Queue::wake(Elem* e, int ax){
		e->pcb->blockedOn=0;             //podesavanja parametara pcb-a
		editAX(e->pcb,ax);
		e->pcb->status=ready_wokenUp;
		myKernelSem->incVal();
		Scheduler::put(e->pcb);

		if (e->prev)e->prev->next=e->next; //izbacivanje cur iz reda
		if (e->next)e->next->prev=e->prev;
		
		if (e == first) first = e->next;
		if (first==0) last = 0;
}

void Queue::addLast(Elem* newElem){
	lock();
	if(!first){	
		first=newElem;
		first->next=first->prev=0;
		last=first;
	}else{
		last->next=newElem;
		newElem->prev=last;
		newElem->next=0;
		last = newElem;
	}
	unlock();
}

void Queue::addBefore(Elem* newElem, Elem* before){
	if(before->prev)before->prev->next=newElem;
	newElem->prev=before->prev;
	before->prev=newElem;
	newElem->next=before;
	if(before==first){
		first = before->prev;
	}
}

void Queue::addAfter(Elem* after, Elem* newElem){
	newElem->next=after->next;
	if(after->next)after->next->prev=newElem;
	after->next=newElem;
	newElem->prev=after;
	if (after==last)last = newElem;
}

Queue::~Queue(){
	lock();
	for (Elem* cur = first; cur; cur=cur->next){
		wake(cur,0);
		delete cur;
	}
	unlock();
}