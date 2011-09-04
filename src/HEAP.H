//File: Heap.h

#ifndef _HEAP_H_
#define _HEAP_H_


class PCB;

class Heap{
public:
	void insert(PCB* pcb, int key);
	void remove(Elem* e)

private:
	void siftUp(Elem* e);
	void siftDown(Elem* e);
	
	class Elem{
		int prio;
		PCB* pcb;
		Elem* childL, *childR, *parent;
	};
	Elem* head; *tail;
};

#endif


/*--------------------------------
				/ 0\   <---HEAD
		      /     \
			 0<----->0
		   /  \     /  
		  0<-->0<->0<-TAIL
*///------------------------------