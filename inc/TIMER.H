//File Timer.h
#ifndef _TIMER_H_
#define _TIMER_H_


void interrupt timer();
void tick();

extern volatile long int time;

extern volatile int cnt;

void timerInic();
void timerRestore();

extern void interrupt (*oldTimerHandler)();

#endif _TIMER_H_
