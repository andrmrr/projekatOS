/*
 * krnEv.h
 *
 *  Created on: Jun 30, 2021
 *      Author: OS1
 */

#ifndef KRNEV_H_
#define KRNEV_H_

#include "event.h"
#include "pcb.h"

class IVTEntry;

class KernelEv {
public:
	KernelEv(IVTNo ivtNo, Event* ev);
	~KernelEv();
	void wait();
	void signal();

private:
	friend class IVTEntry;
	Event* myEvent;
	PCB* owner;
	int value;
	IVTNo ivteNo;

};



#endif /* KRNEV_H_ */
