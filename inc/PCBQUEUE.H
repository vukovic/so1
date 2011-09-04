//File: PCBQueue.h

#ifndef _PCBQUEUE_H_
#define _PCBQUEUE_H_

typedef int ID;
typedef char* TName;

class PCB;


class PCBQueue{
protected:
	class Elem{
		PCB* pcb;
		Elem *next, *prev;
	public:
		friend class PCBQueue;
		Elem(PCB* newPCB);
	};
public:
	friend class Elem;
	PCBQueue& addLast(PCB* newPCB);
	PCB*  getPCBbyId(ID id);
	PCB* getPCBbyName(TName name);
	PCBQueue& remove(PCB* pcbToRemove);
	PCBQueue(){first=0;last=0;}
private:
	Elem *first, *last;
};
#endif