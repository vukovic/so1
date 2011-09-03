//File: Semaphor.cpp

#include <SEMAPHOR.H>
#include <KRNLSEM.H>
#include <DISPATCH.H>
#include <LOCK.H>


Semaphore::Semaphore(int init){
	lock();
	myImpl = new KernelSem(init);
	unlock();
}
Semaphore::~Semaphore(){
	delete myImpl;
}
int Semaphore::wait(Time maxTimeToWait){
	lock();
	int ret = 1;
	if(myImpl->decVal()<0){
		ret = myImpl->block(maxTimeToWait);
	}
	unlock();
	if(semPreempt)dispatch();
	return ret;
}
void Semaphore::signal(){
	lock();
	if(myImpl->incVal()<0)myImpl->deblock();;
	unlock();
	if(semPreempt)dispatch();
}
int Semaphore::val()const{
	return myImpl->getVal();
}