//File: Timer.cpp

#include <TIMER.H>
#include <PCB.H>
#include <DISPATCH.H>
#include <SCHEDULE.H>
#include <KRNLSEM.H>
#include <SYSIDLE.H>
#include <LOCK.H>

#define SWITCH() 		asm{\
								mov tsp, sp\
								mov tbp, bp\
								mov tss, ss\
							}\
							if (current!=0){\
								current->sp = tsp;\
								current->ss = tss;\
								current->bp = tbp;\
							}\
							tsp = running->sp;\
							tss = running->ss;\
							tbp = running->bp;\
							asm{\
								mov ss, tss\
								mov sp, tsp\
								mov bp, tbp \
							}

PCB* current=0;

int cnt=0;

unsigned   tsp, tbp, tss;

void interrupt (*oldTimerHandler)();

void interrupt timer(){
	if (!cntxSwitchReq){
		asm pushf
		(*oldTimerHandler)();
		cnt--;
		PCB::tick();
		tick();
	}
	if (cnt ==0 || cntxSwitchReq){ 
		cntxSwitchReq=0;
		if (!locked){
			if(running && running->status!=blocked && !running->finished && running->myThread !=sysIdle){
				running->status = ready;
				Scheduler::put((PCB*)running);
			}
			current = (PCB*)running;
			running = Scheduler::get();
			if (running == 0){ //ako nema spremnih to znaci da su sve niti blokirane pa se poziva sys_idle nit
				running = sysIdle->pcb();          //aktivira se sys_idle
			}
			running->status=runner;
			cnt=running->timeSlice;
			SWITCH()
		}else{
			cntxSwitchReq = 1;
		}
	}
}