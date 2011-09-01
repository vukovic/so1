//File: Event.cpp

#include <EVENT.H>
#include <IVTENTRY.H>
#include <LOCK.H>
#include <KERNELEV.H>

Event::Event(IVTNo ivtNo){
	myImpl = new KernelEv(ivtNo);
	IVTEntry *myEntry;
	if ((myEntry=IVTEntry::getEntry(ivtNo))!=0)myEntry->insertListener(this);
}
Event::~Event(){
	IVTEntry *myEntry;
	if((myEntry=IVTEntry::getEntry(myImpl->getIVTNo()))!=0)myEntry->removeListener(this);
}
int Event::wait(int maxTimeToWait){
	lock()
	int ret = 5;
	if (!myImpl->valid()) return 0;
	if (!myImpl->getFlag() /*|| myImpl->decVal()<0*/){
		myImpl->decVal();
		ret=myImpl->block(maxTimeToWait);
	}
	else{
		myImpl->resetFlag();
	}
	unlock()
	return ret;
}
void Event::signal(){
	lock()
	if (myImpl->incVal()<0)myImpl->deblock();
	else {
		myImpl->setFlag();
		myImpl->init(0);
	}
	unlock()
}