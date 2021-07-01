/*
 * krnEv.cpp
 *
 *  Created on: Jun 30, 2021
 *      Author: OS1
 */

#include "SCHEDULE.H"
#include "krnEv.h"
#include "general.h"
#include <DOS.H>

KernelEv::KernelEv(IVTNo ivtNo, Event* ev) : myEvent(ev), ivteNo(ivtNo), owner((PCB*)PCB::running), value(0) {}

KernelEv::~KernelEv(){
	IVTEntry* ivtentry = IVTEntry::ivteArr[ivteNo];
#ifndef BCC_BLOCK_IGNORE
	lock;
	setvect(ivtentry->ivtNum, ivtentry->oldISR);
	unlock;
#endif
	owner = 0;
	myEvent = 0;
}

void KernelEv::wait(){
	if(owner == PCB::running){
		if(value == 0){
			PCB::running->status = PCB::BLOCKED;
			dispatch();
		}
		else{
#ifndef BCC_BLOCK_IGNORE
		lock;
			value = 0;
		unlock;
#endif
		}
	}
}

void KernelEv::signal(){
	if(value == 0){
		if(owner->status == PCB::BLOCKED){
#ifndef BCC_BLOCK_IGNORE
			lock;
			owner->status = PCB::READY;
			Scheduler::put(owner);
			unlock;
#endif
			dispatch();
		}
		else{
#ifndef BCC_BLOCK_IGNORE
			lock;
			value = 1;
			unlock;
#endif
		}
	}

}
