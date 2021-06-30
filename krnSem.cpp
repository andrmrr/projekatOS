/*
 * krnSem.cpp
 *
 *  Created on: Jun 29, 2021
 *      Author: OS1
 */




//#include "pcb.h"
#include "krnSem.h"
//#include "baseList.h"
#include "general.h"
#include "SCHEDULE.H"


BaseList<KernelSem*> KernelSem::kernelSemList;


KernelSem* KernelSem::createKernelSem(Semaphore* s, int init){
	KernelSem* newks = new KernelSem(s, init);
	kernelSemList.add(newks);
	return newks;
}


KernelSem::KernelSem(Semaphore* s, int init){
	value = init;
	sem = s;
}


KernelSem::~KernelSem(){
	cout << "Usli u destruktor KernelSema-a\n";
	KernelSem* ks;
	kernelSemList.goToFirst();
	while(kernelSemList.isCurrent()){
		ks = kernelSemList.getCurrent();
		if(this == ks){
			kernelSemList.removeCurrent();
			break;
		}
	}
	cout << "Izasli iz destruktora KernelSem-a\n";
}


int KernelSem::wait(Time maxTimeToWait){
	if(maxTimeToWait == 0){
#ifndef BCC_BLOCK_IGNORE
	lock;
		value--;
		if(value < 0){
			waitSignalList.add((void*)PCB::running);
			PCB::running->status = PCB::BLOCKED;
			dispatch();
		}
		unlock;
#endif
		//kada ponovo dobije procesor ili ako odmah prodje, vraca 1
		return 1;
	}
	else if(maxTimeToWait < 0){
		return 0;
	}
	else{
#ifndef BCC_BLOCK_IGNORE
	lock;
	cout <<"AJDE DA GA STAVIMO\n";
		waitTimeList.add((void*)PCB::running, maxTimeToWait);
		PCB::running->status = PCB::BLOCKED;
		dispatch();
	unlock;
#endif
		return 0;
	}
}



void KernelSem::signal(){
#ifndef BCC_BLOCK_IGNORE
	lock;
	value++;
	if(value <= 0){
		waitSignalList.goToFirst();
		PCB* unblocked = (PCB*)waitSignalList.getCurrent();
		waitSignalList.removeCurrent();
		cout << "SIGNALIZIRAM\n";
		unblocked->status = PCB::READY;
		Scheduler::put(unblocked);
	}
	unlock;
#endif
}



int KernelSem::val() const {
	return value;
}

