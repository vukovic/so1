//File: asm.cpp


#include <asm.h>

unsigned oldISRSeg, oldISROff;
unsigned ISRSeg, ISROff;

/*ne radi sa dx, zamijenjeno dx svuda sa bx*/

void initISR(IVTNo n, unsigned isrOFF, unsigned isrSEG, unsigned& oldOFF, unsigned& oldSEG, void interrupt (*&oldISRptr)()){
	asm pushf
	asm cli
	ISRSeg = isrSEG;
	ISROff = isrOFF;
	asm{
		push es
		push ax
		push bx
		mov ax, 0
		mov es, ax
		mov bx, word ptr n
		shl bx,1
		shl bx,1
		mov ax, word ptr es:bx
		mov word ptr oldISROff, ax
		add bx, 2
		mov ax, word ptr es:bx
		mov word ptr oldISRSeg, ax
		mov ax, word ptr ISRSeg
		mov word ptr es:bx, ax
		sub bx, 2 
		mov ax, word ptr ISROff
		mov word ptr es:bx, ax
		
		mov bx, word ptr n
		add bx, 58h
		shl bx,1
		shl bx,1
		
		mov ax, oldISROff
		mov word ptr es:bx, ax
		add bx,2
		mov ax, oldISRSeg
		mov word ptr es:bx, ax
		
		les bx, oldISRptr
		mov ax, word ptr oldISROff
		mov word ptr es:[bx], ax
		mov ax, word ptr oldISRSeg
		mov word ptr es:[bx+2], ax

		pop bx
		pop ax
		pop es
	}
	oldOFF = oldISROff;
	oldSEG = oldISRSeg;
	asm popf
}
void restoreISR(IVTNo n, unsigned oldOFF, unsigned oldSEG){
	asm pushf
	asm cli
	oldISROff = oldOFF;
	oldISRSeg = oldSEG;
	asm{
		push es
		push ax
		push bx
		mov ax, 0
		mov es, ax
		mov bx, word ptr n
		shl bx,1
		shl bx,1
		mov ax, oldISROff
		mov word ptr es:bx, ax
		add bx, 2
		mov ax, oldISRSeg
		mov word ptr es:bx, ax
		pop bx
		pop ax
		pop es
		popf
	}
}
void editAX(PCB* pcb, int value){
	asm pushf
	asm cli
	unsigned temp_sp,temp_ss;
	if(pcb){
		temp_sp=pcb->sp;
		temp_ss=pcb->ss;
	}else return;
	asm{
		push bx
		push es
		push dx
		mov bx, word ptr temp_sp
		mov es, word ptr temp_ss
		mov dx, word ptr value
		mov word ptr es:[bx+16], dx
		pop dx
		pop es
		pop bx
		popf
	}
}