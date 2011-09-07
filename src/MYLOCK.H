//File: MYLOCK.H

#ifndef _INTLOCK_H_
#define _INTLOCK_H_

#define lock(); { asm pushf; asm cli; }
#define unlock(); asm popf;

#endif
