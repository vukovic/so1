//File: MYLOCK.H

#ifndef _INTLOCK_H_
#define _INTLOCK_H_

extern volatile int locked;

extern inline void lock();

void unlock();


#endif
