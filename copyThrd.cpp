/*
 * copyThrd.cpp
 *
 *  Created on: Jul 2, 2021
 *      Author: OS1
 */


#include "copyThrd.h"
#include "pcb.h"
#include <DOS.H>
#include <IOSTREAM.H>
#include "general.h"
#include "SCHEDULE.H"

CopyThread::CopyThread(PCB* p, PCB* c) : Thread(), parent(p), child(c) {}

void CopyThread::run() {
#ifndef BCC_BLOCK_IGNORE
	unsigned* parentBasePointer = (unsigned*)(MK_FP(parent->ss, parent->bp));
	unsigned* parentStackPointer = (unsigned*)(MK_FP(parent->ss, parent->sp));
	unsigned* childStackPointer = child->stack + (parentStackPointer - parent->stack);

	//postavljamo sp  i ss deteta
	child->ss = FP_SEG(childStackPointer);
	child->sp = FP_OFF(childStackPointer);


	//kopiranje steka
	int i;
	unsigned* rbp = parentBasePointer, *nbp = childStackPointer + (parentBasePointer - parentStackPointer);
	//postavljamo bp deteta
	child->bp = FP_OFF(nbp);
	//cout << "rbp: " << rbp <<endl;
	//cout << "rsp: " << parentStackPointer <<endl;
	for(i = 0; (parentStackPointer + i) != (parent->stack + parent->size); i++){
		*(childStackPointer + i) = *(parentStackPointer + i);
		//syncPrintf("i: %d\n", i);
		//provera za bp
		if((parentStackPointer + i) == rbp){
			unsigned* rbp2 = (unsigned*)MK_FP(parent->ss, *rbp);
			*nbp = FP_OFF(nbp + (rbp2 - rbp));
			nbp = (unsigned*)MK_FP(child->ss, *nbp);
			rbp = rbp2;
			//cout << "rbp: " << rbp << endl;
		}
	}

	//lock;
	//child->status = PCB::READY;
	//Scheduler::put(child);
	//unlock;
	child->start();
#endif
}


