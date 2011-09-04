//File Timer.h
#ifndef _TIMER_H_
#define _TIMER_H_


void interrupt timer();
void tick();

extern void interrupt (*oldTimerHandler)();

#endif _TIMER_H_
