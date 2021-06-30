/*
 * krnSem.h
 *
 *  Created on: Jun 29, 2021
 *      Author: OS1
 */

#ifndef KRNSEM_H_
#define KRNSEM_H_


//#include "thread.h"
#include "baseList.h"
#include "timeList.h"
#include "semaphor.h"
//#include "wsList.h"
#include "pcb.h"

class PCB;



class KernelSem {

public:
	static KernelSem* createKernelSem(Semaphore* sem, int init = 1);
	 ~KernelSem();

	int wait(Time maxTimeToWait);
	void signal();

	int val() const;

	static BaseList<KernelSem*> kernelSemList;

	TimeList waitTimeList;
private:

	//obavezno kastuj na PCB*
	BaseList<void*> waitSignalList;
	int value;
	Semaphore* sem;

protected:
	KernelSem(Semaphore* s, int init = 1);


};




#endif /* KRNSEM_H_ */
