//File: OSMain.cpp

#include <SYSIDLE.H>
#include <TIMER.H>
#include <THREAD.H>
#include <DISPATCH.H>
#include <PCB.H>
#include <LOCK.H>
#include <asm.h>
#include <dos.h>
#include <IVTENTRY.H>


int userMain(int numArgs, char* vArgs[] );

class UMain : public Thread{
public:
	UMain(int _numArgs, char* _vArgs[] ): Thread("user_main_stream"){
		vArgs=_vArgs;
		numArgs=_numArgs;
		start();
	}
	int join();
	~UMain(){
		waitToComplete();
	}
protected:
	virtual void run(){
		retValue=userMain(numArgs,vArgs);
	}
private:
	char** vArgs;
	int numArgs, retValue;
};

int UMain::join(){
	waitToComplete();
	return retValue;
}

unsigned oldTimerOFF, oldTimerSEG;

int main(int numArgs, char* vArgs[] ){
	lock()
	//cout<<"\nOS Initializing... \n";
	PCB* main_pcb = new PCB("main_stream",defaultStackSize,defaultTimeSlice,0);
	running = main_pcb;
	sysIdle = new SystemIdle();
	UMain *mainThread = new UMain(numArgs,vArgs);
	initISR(8,FP_OFF(timer),FP_SEG(timer),oldTimerOFF,oldTimerSEG,oldTimerHandler);
	//cout<<"\nUSER APP about to start... \n";
	unlock()
	
	int ret = mainThread->join();
	
	lock()
	//cout<<"\nUSER APP ended... \nRESTORING... \n";
	sysIdle->stop();

	restoreISR(8,oldTimerOFF, oldTimerSEG);
	
	delete sysIdle;
	delete mainThread;
	
	main_pcb->finished=1;
	delete main_pcb;
	//cout<<"\nAll done! Bye!\n";
	return ret;
}