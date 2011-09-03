//File: Queue.cpp
#include <QUEUE.H>
#include <KRNLSEM.H>
#include <SCHEDULE.H>
#include <PCB.H>
#include <LOCK.H>



/*
	Ova f-ja dodaje pcb u red blokiranih odrzavajuci neopadajuci poredak po vremenu
	koje nit moze da provede blokirana, s tim sto se niti koje mogu neograniceno da 
	ostanu blokirane stavljaju na kraj reda.
*/


Queue& Queue::put(PCB* pcb, int timeToWait){ 
	lock();
	Data* newData = new Data(pcb, timeToWait);
	Elem* newElem = new Elem(newData);
	if(!first){
		first=newElem;
		first->next=first->prev=0;
		last=first;
	}else if (newData->value==0)addLast(newElem);
	else {
		Elem* cur = first;
		for (; cur->next && cur->data->value <= newData->value
								&& cur->next->data->value!=0;cur=cur->next);
		if (cur->data->value>newData->value){
			if(cur->prev)cur->prev->next=newElem;
			newElem->prev=cur->prev;
			cur->prev=newElem;
			newElem->next=cur;
		}else if(!cur->next)addLast(newElem);
		else{
			newElem->next=cur->next;
			if(cur->next)cur->next->prev=newElem;
			cur->next=newElem;
			newElem->prev=cur;
		}
	}
	unlock();
	return *this;
}

/*vraca element koji je najduze u redu.*/
PCB* Queue::get(){
	lock();
	Elem* cur = first;
	if (first)first=first->next;
	if(first){
		first->prev=0;
		if (first->data->value)first->data->value-=passed;
	}else last = 0;
	passed=0;
	PCB* pcb = cur?cur->data->pcb:0;
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
	Elem* cur = first, *temp;
	for(;cur && cur->data->pcb !=pcb; cur=cur->next);
	if ((temp=cur)!=0){//ako je nadjen pcb
		do{
			temp->data->pcb->blockedOn=0;             //podesavanja parametara pcb-a
			if (byTimer) editAX(temp->data->pcb,2);
			else editAX(temp->data->pcb,0);
			temp->data->pcb->status=ready_wokenUp;
			myKernelSem->incVal();
			Scheduler::put(temp->data->pcb);

			if (temp->prev)temp->prev->next=temp->next; //izbacivanje cur iz reda
			if (temp->next)temp->next->prev=temp->prev;

			Elem* prev = temp;                        //korak iteracije
			temp=temp->next;
			if (first && temp==first->next)first=temp;

			delete prev;                              //brisanje elementa koji je uklonjen

		}while(temp && temp->data->value && temp==first && (temp->data->value-=passed)<=0); //na sledeci element se prelazi ako ima jos elemenata, ako sledeci element nije "nula" element, ako je ciljani element bio prvi i ako je proslo dovoljno vremena da se i sledeci stavi u red spremnih.
	}
	if (byTimer || cur!=first)passed = 0;                     //passed se ponistava ako je timer trazio izbacivanje, ili ako je wakeUp trazio izbacivanje prvog iz reda.
	if (!first) last=0;
	unlock()
}

void Queue::addLast(Elem* newElem){
	lock();
	last->next=newElem;
	newElem->prev=last;
	newElem->next=0;
	last = newElem;
	unlock();
}
Queue::~Queue(){
	lock();
	for (Elem* cur = first; cur; cur=cur->next){
		delete cur->data;
		delete cur;
	}
	unlock();
}