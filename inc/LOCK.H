//File: intLock.h

#ifndef _INTLOCK_H_
#define _INTLOCK_H_

#define lock() { asm pushf; asm cli; }
#define unlock() asm popf;

#endif


//valjalo bi mozda isprobati sa metodom zakljucavanja iz z5n.cpp