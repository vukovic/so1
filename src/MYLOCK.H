//File: MYLOCK.H

#ifndef _INTLOCK_H_
#define _INTLOCK_H_

//#define lock(); { asm pushf; asm cli; }
//#define unlock(); asm popf;

#include <DISPATCH.H>
#include <TIMER.H>

volatile extern int locked;

void lock();

void unlock();

#endif


//valjalo bi mozda isprobati sa metodom zakljucavanja iz z5n.cpp