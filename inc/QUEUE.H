//File: Queue.h

#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef unsigned int Time;
class PCB;
class KernelSem;

struct Data{
	/*Jedan element ovog reda ce ciniti pcb, sto predstavlja pcb procesa koji je blokiran, i value, sto 
		predstavlja max vrijeme koje proces moze biti blokiran.*/
	PCB* pcb;
	int value;
	Data(PCB* _pcb, int val){
		pcb=_pcb;
		value=val;
	}
};

class Queue{
private:
	class Elem{
		Data* data;
		Elem *next, *prev;
	public:
		friend class Queue;
		friend class KernelSem;
		Elem(Data *newData){data = newData;}
	};
	Elem *first, *last;
	Time passed;//vrijeme koje je proteklo od kad je doslo do promjene prvog elementa reda do sad.
	void addLast(Elem*);//pomocna f-ja koja dodaje elem na kraj reda!
	KernelSem* myKernelSem;
public:
	friend class KernelSem;
	Queue(KernelSem* ks){first=last=0;myKernelSem=ks;}
	~Queue();
	
	Queue& put(PCB* pcb, int timeToWait); //Smijesta novi element u red odrzavajuci neopadajuci redosled vremena blokiranja.
	PCB* get();//Uzima prvi element na koji naidje
	void remove (PCB* pcbToRemove, int byTimer);//Uklanja element sa zadatim pcb-om iz reda
};
#endif