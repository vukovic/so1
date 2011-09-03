//File: LOCK.CPP
#include <LOCK.H>

inline void lock(){locked=1;}

void unlock(){
	asm cli
	locked=0;
	if (cntxSwitchReq){
		timer();
		cnt=1;
	}
	asm sti
}
