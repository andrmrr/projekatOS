/*
 * krnSem.h
 *
 *  Created on: Jul 1, 2021
 *      Author: OS1
 */

#ifndef KRNSEM_H_
#define KRNSEM_H_

#include "pcb.h"


class KernelSem {

public:
	KernelSem(int init = 1);
	~KernelSem();

	int wait(Time maxTimeToWait);
	void signal();

	int val() const;




	struct ElemSig{
		PCB* data;
		ElemSig* next;
	};

	struct ElemTime{
		PCB* data;
		ElemTime* next;
		Time tm;
	};


	ElemSig* sigHead, *sigTail;
	ElemTime* timeHead;
	int value;

};


struct KernelSemElem {
	KernelSem* ks;
	KernelSemElem * next;
};

extern KernelSemElem* ksHead;

#endif /* KRNSEM_H_ */
