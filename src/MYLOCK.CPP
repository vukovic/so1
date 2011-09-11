//File: LOCK.CPP
#include <MYLOCK.H>
#include <DISPATCH.H>
#include <TIMER.H>

volatile int locked = 0;

void lock(){locked=1;}

void unlock(){
	asm cli
	locked=0;
	if (cntxSwitchReq){
		timer();
	}
	asm sti
}
