//File: Timer.cpp

#include <TIMER.H>
#include <PCB.H>
#include <DISPATCH.H>
#include <SCHEDULE.H>
#include <KRNLSEM.H>
#include <SYSIDLE.H>
#include <LOCK.H>

PCB* current=0;

int cnt=0;

unsigned   tsp, tbp, tss;

void interrupt (*oldTimerHandler)();

void interrupt timer(){
	asm cli
	if (!cntxSwitchReq){
		(*oldTimerHandler)();
		cnt--;
		PCB::tick();
		tick();
	}
	if (cnt <=0 || cntxSwitchReq){ // da li <= ili ==
		cntxSwitchReq=0;
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
		asm{
			mov tsp, sp
			mov tbp, bp
			mov tss, ss
		}
		if (current!=0){
			current->sp = tsp;
			current->ss = tss;
			current->bp = tbp;
		}
		tsp = running->sp;   
		tss = running->ss;	
		tbp = running->bp;
		asm{
			mov ss, tss
			mov sp, tsp
			mov bp, tbp 
		}
	}
}

/*
int oldTimerOFF, oldTimerSEG;
void timerInic(){
	lock()
	asm{
		push es
		push ax
		mov ax,0
		mov es,ax
		mov ax, word ptr es:0022h        //ovaj kod
		mov word ptr oldTimerSEG, ax     //postavlja oldTimerSEG = [0022h]
		mov ax, word ptr es:0020h        //ovaj kod
		mov word ptr oldTimerOFF, ax     //postavlja oldTimerOFF = [0020h]
		mov word ptr es:0022h, seg timer        //ovaj kod
		mov word ptr es:0020h, offset timer     //postavlja moju pr. rut. na 08h
		mov ax,oldTimerSEG                 //
		mov word ptr es:0182h, ax          //ovaj kod
		mov ax, oldTimerOFF                //postavlja staru rut. na 60h
		mov word ptr es:0180h, ax          //
		pop ax
		pop es
	}
	unlock()
}
void timerRestore(){
	lock()
	asm{
		push es
		push ax
		mov ax,0
		mov es, ax
		mov ax, oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0020h, ax
		pop ax
		pop es
	}
	unlock()
}
*/