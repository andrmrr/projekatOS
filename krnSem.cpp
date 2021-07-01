/*
 * krnSem.cpp
 *
 *  Created on: Jun 29, 2021
 *      Author: OS1
 */


#include "krnSem.h"
#include "general.h"
#include "SCHEDULE.H"


BaseList<KernelSem*> KernelSem::kernelSemList;

/*
KernelSem* KernelSem::createKernelSem(Semaphore* s, int init){
	//cout << "Pravi se semafor!\n";
	KernelSem* newks;
#ifndef BCC_BLOCK_IGNORE
	lock;
	newks = new KernelSem(s, init);
	kernelSemList.add(newks);
	unlock;
#endif
	return newks;
}
*/

KernelSem::KernelSem(int init){
	value = init;
#ifndef BCC_BLOCK_IGNORE
	lock;
	kernelSemList.add(this);
	unlock;
#endif
}


KernelSem::~KernelSem(){
#ifndef BCC_BLOCK_IGNORE
	lock;
	//cout << "Usli u destruktor KernelSema-a\n";
	KernelSem* ks;
	kernelSemList.goToFirst();
	while(kernelSemList.isCurrent()){
		ks = kernelSemList.getCurrent();
		if(this == ks){
			kernelSemList.removeCurrent();
			break;
		}
	}
	//cout << "Izasli iz destruktora KernelSem-a\n";
	unlock;
#endif
}


int KernelSem::wait(Time maxTimeToWait){
	//syncPrintf("Nit %d usla u wait semafora\n", PCB::running->getRunningId());
	if(maxTimeToWait == 0){
#ifndef BCC_BLOCK_IGNORE
	lock;
		value--;
		if(value < 0){
			//syncPrintf("AJDE DA GA STAVIMO\n");
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
	//cout <<"AJDE DA GA STAVIMO\n";
		waitTimeList.add((void*)PCB::running, maxTimeToWait);
		PCB::running->status = PCB::BLOCKED;
		dispatch();
	unlock;
#endif
		return 0;
	}
}



void KernelSem::signal(){
	//syncPrintf("Nit %d je usla u signal\n", PCB::running->getRunningId());
#ifndef BCC_BLOCK_IGNORE
	lock;
	value++;
	if(value <= 0){
		waitSignalList.goToFirst();
		PCB* unblocked = (PCB*)waitSignalList.getCurrent();
		waitSignalList.removeCurrent();
		//syncPrintf("AJDE DA GA VADIMO\n");
		//cout << "SIGNALIZIRAM\n";
		unblocked->status = PCB::READY;
		Scheduler::put(unblocked);
	}
	unlock;
#endif
}



int KernelSem::val() const {
	return value;
}

