/*
 * IVTEntry.h
 *
 *  Created on: Jun 30, 2021
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "krnEv.h"

typedef void interrupt (*pInterrupt)(...);

class IVTEntry {
public:
	static IVTEntry* ivteArr[256];

	IVTEntry(IVTNo ivtNo, pInterrupt newIntr);
	~IVTEntry();
	void signal();
	void setMyKernelEv(KernelEv* ke);

	pInterrupt oldISR;//, newISR;
	IVTNo ivtNum;
private:
	friend class KernelEv;
	KernelEv* myKernelEv;
};


#endif /* IVTENTRY_H_ */
