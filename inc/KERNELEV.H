//File: KernelEv.h

#ifndef _KERNELEV_H_
#define _KERNELEV_H_
#include <DISPATCH.H>
#include <PCB.H>
#include <KRNLSEM.H>

typedef unsigned char IVTNo;

class KernelEv: public KernelSem{
public:
	KernelEv(IVTNo ivtNo):KernelSem(0){
		flag = 0;
		this->ivtNo = ivtNo;
		myPCB=(PCB*)running;
	}
	~KernelEv(){}
	
	int valid(){return running == myPCB;}
	
	int getFlag()const{return flag;}
	void setFlag(){flag=1;}
	void resetFlag(){flag=0;}
	
	IVTNo getIVTNo()const{return ivtNo;}
private:
	IVTNo ivtNo;
	int flag;
	PCB* myPCB;
};


#endif