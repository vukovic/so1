//File: Dispatch.h

#ifndef _DISPATCH_H_
#define _DISPATCH_H_

class PCB;

extern volatile PCB* running;
extern volatile int cntxSwitchReq;
extern volatile int cntxSwitchAllowed;

void dispatch();
#endif