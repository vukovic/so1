//File: heap.h

#ifndef _HEAP_H_
#define _HEAP_H_

#include <MYLOCK.H>	

class KernelSem;
class PCB;

class Heap{
public:
		void write();
	Heap(KernelSem *ks):myKernelSem(ks),last(0),capacity(30), timeNow(0){
		lock();
		heap = new Elem[capacity];
		unlock();
	}
	~Heap(){delete [] heap;}

	void put (PCB* pcb, int prio);
	PCB* get();
	void remove (PCB* pcb, int byTimer);

	PCB* getHeadPCB(){return (last>=1)?heap[1].pcb:0;}
	int incTime(){return ++timeNow && last>=1 && heap[1].prio && ((timeNow - heap[1].time0)>=heap[1].prio);}
protected:
	class Elem{
	public:
		Elem(PCB* _pcb, int _prio, unsigned long int _time0, Heap* _myHeap):pcb(_pcb),prio(_prio),time0(_time0),myHeap(_myHeap){}
		Elem():pcb(0),time0(0),myHeap(0),prio(0){}
		friend int operator<(Elem& e1, Elem& e2);
		friend int operator>(Elem& e1, Elem& e2);
		friend int operator== (Elem& e1, Elem& e2);
		int priority();

		friend class Heap;
	private:
		Heap* myHeap;
		PCB* pcb;
		int prio;
		unsigned long int time0;
	};
private:
	PCB* remove (int indx = 1);
	void wake(int indx, int ax);
	
	int propellUp(int indx);
	int propellDown(int indx);
	
	int lChild(int indx){return 2*indx;}
	int rChild(int indx){return 2*indx+1;}
	int parent(int indx){return indx/2;}
	
	KernelSem *myKernelSem;
	Elem *heap;
	int last;
	int capacity;
	unsigned long int timeNow;
	
	friend class Elem;
};

#endif