/*
 * general.h
 *
 *  Created on: Jun 23, 2021
 *      Author: OS1
 */

#ifndef GENERAL_H_
#define GENERAL_H_

#include "thread.h"
#include "pcb.h";
#include "userThrd.h";
#include "IVTEntry.h"

//zakljucavanje i otkljucavanje
#define lock asm { \
	pushf; \
	cli; \
	} \

#define unlock asm popf;

//sinhroni ispis
int syncPrintf(const char *format, ...);

//globalne promenljive
extern Thread* userMainThread;
extern PCB* idlePCB;


//makro za pripremu IVT ulaza
#define PREPAREENTRY(numEntry, callOld)\
	void interrupt inter##numEntry(...);\
	IVTEntry ivte##numEntry(numEntry, inter##numEntry);\
	void interrupt inter##numEntry(...){\
		ivte##numEntry.signal();\
		if(callOld == 1){\
			ivte##numEntry.oldISR();\
		}\
	} \




#endif /* GENERAL_H_ */
