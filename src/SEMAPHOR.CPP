//File: Semaphor.cpp

#include <SEMAPHOR.H>
#include <KRNLSEM.H>
#include <DISPATCH.H>
#include <MYLOCK.H>


Semaphore::Semaphore(int init){
	lock();
	myImpl = new KernelSem(init);
	unlock();
}
Semaphore::~Semaphore(){
	lock();
	delete myImpl;
	unlock();
}
int Semaphore::wait(Time maxTimeToWait){
	lock();
	int ret = 1;
	if(myImpl->decVal()<0){
		ret = myImpl->block(maxTimeToWait);
	}else if(semPreempt)dispatch();
	unlock();
	return ret;
}
void Semaphore::signal(){
	lock();
	if(myImpl->incVal()<0)myImpl->deblock();;
	if(semPreempt)dispatch();
	unlock();
}
int Semaphore::val()const{
	return myImpl->getVal();
}