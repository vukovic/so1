// File: event.h

/*
	Klasa Event je nesto kao binarni semafor. Sustina je da se nit zablokira, a da je odblokira prekidna
	rutina odgovarajuceg objekta klase IVTEntry.
*/
//------------------------------------------------------------------------------------
#ifndef _EVENT_H_
#define _EVENT_H_
typedef unsigned char IVTNo;

class KernelEv;

class Event {
public:
   Event (IVTNo ivtNo);
  ~Event ();

   int wait  (int maxTimeToWait);
   void signal();

private:
   KernelEv* myImpl;
};
#endif