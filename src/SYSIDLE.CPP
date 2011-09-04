//File: SystemIdle.cpp

#include <SYSIDLE.H>
#include <THREAD.H>
#include <SEMAPHOR.H>
#include <DISPATCH.H>
#include <SCHEDULE.H>
#include <MYLOCK.H>
#include <PCB.H>

volatile int systemIdleThreadStop=0;

SystemIdle* sysIdle = 0;

void SystemIdle::run(){
	while(!systemIdleThreadStop);
};

SystemIdle::SystemIdle():Thread("sys_idle",defaultStackSize,1){
	sysIdlePCB=PCB::getPCBbyName("sys_idle");
}
PCB* SystemIdle::pcb(){
	return sysIdlePCB;
}
void SystemIdle::stop(){
	systemIdleThreadStop=1;
	waitToComplete();
}
SystemIdle::~SystemIdle(){
	systemIdleThreadStop=1;
	waitToComplete();
};