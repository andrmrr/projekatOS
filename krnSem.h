/*
 * krnSem.h
 *
 *  Created on: Jun 29, 2021
 *      Author: OS1
 */

#ifndef KRNSEM_H_
#define KRNSEM_H_


#include "baseList.h"
#include "timeList.h"
#include "semaphor.h"
#include "pcb.h"



class KernelSem {

public:
	KernelSem(int init = 1);
	 ~KernelSem();
	int wait(Time maxTimeToWait);
	void signal();
	int val() const;

	static BaseList<KernelSem*> kernelSemList;

	//obavezno kastuj na PCB*
	TimeList waitTimeList;
	BaseList<void*> waitSignalList;

private:
	int value;

};




#endif /* KRNSEM_H_ */
