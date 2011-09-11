//File: heap.cpp

#include <HEAP.H>
#include <asm.h>
#include <MYLOCK.H>

void Heap::insert(PCB* pcb, int prio){
	lock();
	int indx=++last;
	if((indx >= capacity){
		Elem* _heap = new Elem[capacity*=2];
		for(int i = 1; i<capacity/2;i++)_heap[i]=heap[i];
		delete [] heap;
		heap = _heap;
	}
	heap[indx] = Elem(pcb,prio,timeNow);
	propellUp(indx);
	unlock();
}

Heap::Elem& Heap::remove(int indx = 1){
	lock();
	Elem* toRemove = 0;
	if (last>=1){
		toRemove = &heap[indx];
		heap[indx]=heap[last--];
		propellUp(indx);
		propellDown(indx);
	}
	unlock();
	return toRemove;
}

void Heap::propellUp(int indx){
	while(indx>1 && heap[indx] < heap[parent(indx)]){
		Elem* temp = heap[parent(indx)];
		heap[parent(indx)]=heap[indx];
		heap[indx]=temp;
		indx=parent(indx);
	}
}

void Heap::propellDown(int indx){
	while(((lChild(indx)<=last) && (heap[indx] > heap[lChild(indx)]))||
			((rChild(indx)<=last) && (heap[indx] > heap[rChild(indx)])){
		Elem min(0,0,0);
		if ((lChild(indx)<=last) && (rChild(indx)<=last))min = heap[(lChild(indx) < rChild(indx)) ? lChild(indx):rChild(indx)];
		else min = heap[ (lChild(indx)<=last)? lChild(indx):rChild(indx);
		int indx2 = indx;
		if (min == heap[lChild(indx)]){
			heap[lChild(indx)] = heap[indx];
			indx = lChild(indx);
		}else{
			heap[rChild(indx)] = heap[indx];
			indx = rChild(indx);
		}
		heap[indx2]=min;
	}
}
void Heap::get(){
	Elem* e = remove();
	if (e) return e->pcb;
	else return 0;
}
void Heap::remove(PCB* pcb, int byTimer){
	lock();
	int indx = 1;
	for(; indx <= last && heap[indx]->pcb != pcb; i++);
	if (indx<=last && !byTimer){
		Elem e = remove(indx);
		wake(e,0);
	}else if(indx <= last &&  byTimer){
		do{
			Elem e = remove();
			wake (e,2);
		}while(last>=1 && heap[indx].priority()>=heap[indx].prio);
	}
	unlock();
}

void wake(int indx, int ax){
	heap[indx].pcb->blockedOn=0;             //podesavanja parametara pcb-a
	editAX(heap[indx].pcb,ax);
	heap[indx].pcb->status=ready_wokenUp;
	myKernelSem->incVal();
	Scheduler::put(heap[indx].pcb);
}

inline int Heap::Elem::priority(){
	return (myHeap->timeNow - time0);
}

bool operator<(Heap::Elem& e1, Heap::Elem& e2){
	return (!e2.prio && e1.prio) || (e1.prio && (e1.priority()<e2.priority()));
}
bool operator>(Heap::Elem& e1, Heap::Elem& e2){
	return (!e1.prio && e2.prio) || (e2.prio && (e1.priority()>e2.priority()));
}
bool operator==(Heap::Elem& e1, Heap::Elem& e2){
	return e1.pcb == e1.pcb;
}