/*
 * IVTEntry.cpp
 *
 *  Created on: Jun 30, 2021
 *      Author: OS1
 */

#include "IVTEntry.h"
#include "general.h"
#include <DOS.H>
#include <IOSTREAM.H>

IVTEntry* IVTEntry::ivteArr[256] = {0};

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newIntr){
#ifndef BCC_BLOCK_IGNORE
	lock;
	oldISR = getvect(ivtNo);
	setvect(ivtNo, newIntr);
	unlock;
#endif
	myKernelEv = 0;
	ivtNum = ivtNo;
	ivteArr[ivtNo] = this;
}

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	lock;
	setvect(ivtNum, oldISR);
	unlock;
#endif
	ivteArr[ivtNum] = 0;
}


void IVTEntry::signal(){
	if(myKernelEv){
		myKernelEv->signal();
	}
}

void IVTEntry::setMyKernelEv(KernelEv* ke){
	myKernelEv = ke;
}
