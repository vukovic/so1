//File: Dispatch.cpp

#include <DISPATCH.H>
#include <TIMER.H>
#include <PCB.H>
#include <LOCK.H>

volatile PCB* running = 0;
volatile int cntxSwitchReq=0;

void dispatch(){
	lock()
	cntxSwitchReq=1;
	timer();
	unlock()
}