//File: LOCK.CPP
#include <MYLOCK.H>

volatile int locked = 0;

inline void lock(){locked=1;}

void unlock(){
	asm cli
	locked=0;
	if (cntxSwitchReq){
		timer();
	}
	asm sti
}
