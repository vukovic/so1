//File: SemQue.h


//Mislim da ovo nije potrebno, vec da treba prolaziti kroz allPCBs i gdje je blockedOn!=0 pozvati blockedOn.tick()!

#ifndef _SEMQUE_H_
#define _SEMQUE_H_
class KernelSem;

class KrnlSemQueue{
protected:
	class Elem{
		KernelSem* ks;
		Elem *next, *prev;
	public:
		friend class KrnlSemQueue;
		Elem(KernelSem* newKs);
	};
public:
	friend class Elem;
	KrnlSemQueue& addLast(KernelSem* newKs);
	KrnlSemQueue& remove(KernelSem* krnlSemToRemove);
	KrnlSemQueue(){first=0;last=0;}
	void tick();
private:
	Elem* first, *last;
};
#endif