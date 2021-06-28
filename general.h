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

//zakljucavanje i otkljucavanje
#define lock asm { \
	pushf; \
	cli; \
	} \

#define unlock asm popf;

extern Thread* userMainThread;
extern PCB* idlePCB;




#endif /* GENERAL_H_ */
