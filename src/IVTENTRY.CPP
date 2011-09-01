//File: IVTENTRY.CPP

#include <IVTENTRY.H>
#include <asm.h>
#include <EVENT.H>
IVTEntry* allEntries[256];


IVTEntry::Elem::Elem(Event* event, IVTEntry* outClass){
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
}

IVTEntry::IVTEntry(IVTNo n, unsigned isrOFF, unsigned isrSEG, int callOld){
	initISR(n,isrOFF,isrSEG,oldOFF,oldSEG, oldHandler);
	this->callOld = callOld;
	allEntries[n]=this;
}
IVTEntry::~IVTEntry(){
	restoreISR(n, oldOFF, oldSEG);
}
void IVTEntry::interrupted(){
	if (callOld) {
		(*oldHandler)();
	}
	for (Elem* cur = first; cur; cur = cur->next){
		cur->event->signal();
	}
}

void IVTEntry::insertListener(Event* e){
	new Elem(e,this);
}

void IVTEntry::removeListener(Event* e){
	Elem* cur = first;
	for(;cur && cur->event != e; cur = cur ->next);
	if (cur){
		if (cur == first) first = first->next;
		if (first == 0) last = 0;
		if (cur->prev)cur->prev->next = cur->next;
		if (cur->next)cur->next->prev = cur->prev;
	}
}

IVTEntry* IVTEntry::getEntry(IVTNo intNo){
	return allEntries[intNo];
}