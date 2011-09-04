//File: LOCK.CPP
#include <MYLOCK.H>

void lock(){locked=1;}

void unlock(){
	asm cli
	locked=0;
	if (cntxSwitchReq){
		timer();
		cnt=1;
	}
	asm sti
}
