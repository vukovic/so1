//File: IVTEvent.h

/*
	Ovdje treba obezbjediti makro PREPAREENTRY, i klasu IVTEntry, a ovaj fajl treba ukljuciti u EVENT.H
*/

//--------------------------------------------------------------------------------------------------------

/*
	PREPAREENTRY treba da kreira objekat klase IVTEntry za broj ulaza naveden kao prvi argument.
	Kreiranjem objekta klase Event za isti broj ulaza treba da izazove ubacivanje datog objekta u 
	listu koja je clan klase IVTEntry. Pri kreiranju objekta klase IVTEntry treba napraviti i 
	prekidnu rutinu koja odgovara datom ulazu, i u njoj treba/ne treba pozvati prethodnu prekidnu rutinu
	u zavisnosti od drugog parametra makroa PREPAREENTRY. Kada se desi prekid, u pozvanoj prekidnoj rutini
	je potrebno da se pozove metoda interrupted() odgovarajuceg objekta klase IVTEntry, koja ce da prodje
	kroz listu eventova za dati broj ulaza i da pozove signal() na svakom od njih.
	
	U destruktoru klase IVTEntry treba obezbjediti da se restaurira prethodna prekidna rutina.
	
*/
#ifndef _IVTEVENT_H_
#define _IVTEVENT_H_
#include <dos.h>

#define PREPAREENTRY(N,CALLPREV) void interrupt ISR_##N();\
								 IVTEntry IVTEntry##N(N,FP_OFF(ISR_##N),FP_SEG(ISR_##N),CALLPREV);\
								 void interrupt ISR_##N(){\
							 		allEntries[N]->interrupted();\
						   		 }
typedef unsigned char IVTNo;
class IVTEntry;
class Event; 


extern IVTEntry* allEntries[256];

class IVTEntry{
public:
	IVTEntry(IVTNo n, unsigned int isrOFF, unsigned int isrSEG, int callOld);
	~IVTEntry();
	void interrupted();
	void insertListener(Event*);
	void removeListener(Event*);
	static IVTEntry* getEntry(IVTNo);
	void restore();
private:
	int callOld;
	IVTNo n;
	unsigned oldSEG,oldOFF;
	
	void interrupt (*oldHandler)();
	
	class Elem{
		Event* event;
		Elem* prev;
		Elem* next;
	public:
		friend class IVTEntry;
		Elem(Event* event, IVTEntry* outClass);
	};
	Elem* first, *last;
	friend class Elem;
};
#endif