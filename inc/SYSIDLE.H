//File: SysIdle.h
#ifndef _SYSIDLE_H_
#define _SYSIDLE_H_

#include <THREAD.H>
#include <SEMAPHOR.H>

class SystemIdle;
class PCB;
extern SystemIdle *sysIdle;


class SystemIdle : public Thread{
public:
	SystemIdle();
	~SystemIdle();
	void stop();
	PCB* pcb();
protected:
	virtual void run();
private:
	PCB* sysIdlePCB;
};
#endif
