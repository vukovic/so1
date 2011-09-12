//File: heap.cpp

#include <HEAP.H>
#include <asm.h>
#include <MYLOCK.H>
#include <KRNLSEM.H>
#include <PCB.H>
#include <SCHEDULE.H>


void Heap::put(PCB* pcb, int prio){
	lock();
	int indx=++last;
	if(	indx >= capacity){
		Elem* _heap = new Elem[capacity*=2];
		for(int i = 1; i<capacity/2; i++)_heap[i]=heap[i];
		delete [] heap;
		heap = _heap;
	}
	heap[indx] = Elem(pcb,prio,timeNow,this);
	propellUp(indx);
	unlock();
}
PCB* Heap::remove(int indx ){
	lock();
	PCB* toRemove = 0;
	if (last>=1 && indx<=last && indx>=1){
		toRemove = heap[indx].pcb;
		heap[indx]=heap[last--];
		indx=propellUp(indx);
		propellDown(indx);
	}
	unlock();
	return toRemove;
}

int Heap::propellUp(int indx){
	while(indx>1 && (heap[indx] < heap[parent(indx)])){
		Elem temp = heap[parent(indx)];
		heap[parent(indx)]=heap[indx];
		heap[indx]=temp;
		indx=parent(indx);
	}
	return indx;
}

int Heap::propellDown(int indx){
	while(((lChild(indx)<=last) && (heap[indx] > heap[lChild(indx)]))||
			((rChild(indx)<=last) && (heap[indx] > heap[rChild(indx)]))){
		Elem min;
		if ((lChild(indx)<=last) && (rChild(indx)<=last))min = heap[(heap[lChild(indx)] < heap[rChild(indx)]) ? lChild(indx):rChild(indx)];
		else min = heap[ (lChild(indx)<=last)? lChild(indx):rChild(indx)];
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
	return indx;
}
PCB* Heap::get(){
	PCB* pcb = remove();
	return pcb;
}
void Heap::remove(PCB* pcb, int byTimer){
	lock();
	int indx = 1;
	if (last<1)return;
	for(; indx <= last && heap[indx].pcb != pcb; indx++);
	if (indx<=last && !byTimer){
		wake(indx,0);
		remove(indx);
	}else if(indx <= last &&  byTimer){
		do{
			wake (1,2);
			remove();
		}while(last>=1 && heap[indx].prio && (heap[indx].priority()<=0));
	}
	unlock();
}
void Heap::wake(int indx, int ax){
	heap[indx].pcb->blockedOn=0;             //podesavanja parametara pcb-a
	editAX(heap[indx].pcb,ax);
	heap[indx].pcb->status=ready_wokenUp;
	myKernelSem->incVal();
	Scheduler::put(heap[indx].pcb);
}
int Heap::Elem::priority(){
	return (prio - myHeap->timeNow + time0);
}
int operator< (Heap::Elem& e1, Heap::Elem& e2){
	return (!e2.prio && e1.prio) || (e1.prio && (e1.priority()<e2.priority()));
}
int operator> (Heap::Elem& e1, Heap::Elem& e2){
	return (!e1.prio && e2.prio) || (e2.prio && (e1.priority()>e2.priority()));
}
int operator== (Heap::Elem& e1, Heap::Elem& e2){
	return e1.pcb == e2.pcb;
}