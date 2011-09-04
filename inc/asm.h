//File asm.h

#ifndef _ASM_H_
#define _ASM_H_
#include <PCB.H>
#include <MYLOCK.H>


typedef unsigned char IVTNo;

void editAX(PCB* pcb, int value);
void initISR(IVTNo n, unsigned isrOFF, unsigned isrSEG, unsigned& oldOFF, unsigned& oldSEG, void interrupt(*&oldISRptr)());
void restoreISR(IVTNo n, unsigned oldOFF, unsigned oldSEG);

#endif