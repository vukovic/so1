//File: IVTENTRY.CPP

#include <IVTENTRY.H>
#include <asm.h>
#include <EVENT.H>
IVTEntry* allEntries[256];


IVTEntry::Elem::Elem(Event* event, IVTEntry* outClass){
	lock();
	this->event = event;
	if(!outClass->first){
		outClass->first = outClass->last = this;
		prev = next = 0;
	}else{
		outClass->last->next = this;
		prev = outClass->last;
		outClass->last = this;
		next=0;
	}
	unlock();
}

IVTEntry::IVTEntry(IVTNo n, unsigned isrOFF, unsigned isrSEG, int callOld){
	lock();
	initISR(n,isrOFF,isrSEG,oldOFF,oldSEG, oldHandler);
	this->callOld = callOld;
	allEntries[n]=this;
	unlock();
}
IVTEntry::~IVTEntry(){
	
}
void IVTEntry::interrupted(){
	if (callOld) {
		(*oldHandler)();
	}
	lock();
	for (Elem* cur = first; cur; cur = cur->next){
		cur->event->signal();
	}
	unlock();
}

void IVTEntry::insertListener(Event* e){
	lock();
	new Elem(e,this);
	unlock();
}

void IVTEntry::restore(){
	restoreISR(n, oldOFF, oldSEG);
}

void IVTEntry::removeListener(Event* e){
	lock();
	Elem* cur = first;
	for(;cur && cur->event != e; cur = cur ->next);
	if (cur){
		if (cur == first) first = first->next;
		if (first == 0) last = 0;
		if (cur->prev)cur->prev->next = cur->next;
		if (cur->next)cur->next->prev = cur->prev;
		delete cur;
	}
	unlock();
}

IVTEntry* IVTEntry::getEntry(IVTNo intNo){
	return allEntries[intNo];
}