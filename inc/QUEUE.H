//File: Queue.h

#ifndef _QUEUE_H_
#define _QUEUE_H_

class PCB;
class KernelSem;

class Queue{
private:
	class Elem{
		PCB* pcb;
		int prio;
		int time0;
		Elem *next, *prev;
	public:
		friend class Queue;
		Elem(PCB* _pcb, int _prio, int _timeNow):pcb(_pcb),prio(_prio),time0(_timeNow){}	
		int getPrio() const {return prio;}
	};
	Elem *first, *last;
	long int timeNow;									//vrijeme koje je proteklo od kad je doslo do promjene prvog elementa reda do sad.
	int firstPrio;
	KernelSem* myKernelSem;
	
	void addLast(Elem*);						//pomocna f-ja koja dodaje elem na kraj reda!
	void addBefore(Elem*,Elem*);
	void addAfter(Elem*,Elem*);
	void wake(Elem*,int);
public:
	Queue(KernelSem* ks):first(0),last(0),myKernelSem(ks),timeNow(0),firstPrio(0){}
	~Queue();
	
	Queue& put(PCB* pcb, int timeToWait); 		//Smijesta novi element u red odrzavajuci neopadajuci redosled vremena blokiranja.
	PCB* get();									//Uzima element koji moze najkrace da ceka
	void remove (PCB* pcbToRemove, int byTimer);//Uklanja element sa zadatim pcb-om iz reda
	
	int getHeadPrio()const{return first?first->prio:-1;}
	PCB* getHeadPCB(){return first?first->pcb:0;}
	int incTime(){return ++timeNow && first && ((timeNow - first->time0)>=firstPrio) && first->prio; }
};
#endif