/*
 * pcb.h
 *
 *  Created on: Jun 22, 2021
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "baseList.h"


class PCBList;

class PCB {

public:
	enum Status {INITIALIZED, READY, BLOCKED, FINISHED, IDLE};
	unsigned quantum;
	unsigned* stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	Status status;
	Thread* myThread;
	ID id;
	BaseList<PCB*> waitToCompleteList;

	volatile static PCB* running;
	static PCBList pcbList;
	static unsigned unfinished;

	virtual void start();
	virtual void waitToComplete();
	virtual ~PCB();

	virtual ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

	static void wrapper();
	static PCB* createPCB(Thread* mt, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	static PCB* createPCB();
protected:
	PCB(Thread* mt, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	PCB();
	virtual void deblock();

};



#endif /* PCB_H_ */
