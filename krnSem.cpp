/*
 * krnSem.cpp
 *
 *  Created on: Jul 1, 2021
 *      Author: OS1
 */


#include "general.h"
#include "krnSem.h"
#include "SCHEDULE.H"

KernelSemElem* ksHead = 0;

KernelSem::KernelSem(int init){
	value = init;
	timeHead = 0;
	sigHead = 0;
	sigTail = 0;
	KernelSemElem* kse = new KernelSemElem();
	kse->ks = this;
	kse->next = 0;
#ifndef BCC_BLOCK_IGNORE
	lock;
	kse->next = ksHead;
	ksHead = kse;
	unlock;
#endif
}


KernelSem::~KernelSem(){
#ifndef BCC_BLOCK_IGNORE
	lock;
	KernelSemElem* kse = ksHead, *prev = 0;
	while(kse){
		if(kse->ks == this){
			if(prev){
				prev->next = kse->next;
			}
			delete kse;
			break;
		}
		prev = kse;
		kse = kse->next;
	}
	unlock;
#endif
}

int KernelSem::val() const {
	return value;
}

int KernelSem::wait(Time maxTimeToWait){
#ifndef BCC_BLOCK_IGNORE
	lock;
	value--;
	if(value < 0){
		PCB::running->status = PCB::BLOCKED;

		if(maxTimeToWait == 0){
			ElemSig* newes = new ElemSig();
			newes->next = 0;
			newes->data = (PCB*)PCB::running;
			if(!sigHead){
				sigHead = newes;
			}
			else{
				sigTail->next = newes;
			}
			sigTail = newes;
		}
		else{
			ElemTime* newet = new ElemTime();
			newet->next = 0;
			newet->data = (PCB*)PCB::running;
			newet->tm = 0;
			ElemTime* iter = timeHead, *prev = 0;
			while(iter && iter->tm <= maxTimeToWait){
				maxTimeToWait -= iter->tm;
				prev = iter;
				iter = iter->next;
			}
			newet->tm = maxTimeToWait;
			newet->next = iter;
			if(iter){
				iter->tm -= newet->tm;
			}
			if(!prev) {
				timeHead = newet;
			}
			else{
				prev->next = newet;
			}
		}

		dispatch();
	}
	unlock;
#endif
}


void KernelSem::signal(){
#ifndef BCC_BLOCK_IGNORE
	lock;
	value++;
	if(value <= 0){
		PCB* unblocked;
		if(sigHead){
			ElemSig* es = sigHead;
			sigHead = sigHead->next;
			if(!sigHead){
				sigTail = 0;
			}
			unblocked = es->data;
			delete es;
		}
		else{
			if(timeHead){
				ElemTime* et = timeHead;
				timeHead = timeHead->next;
				if(timeHead){
					timeHead->tm += et->tm;
				}
				unblocked = et->data;
				delete et;
			}
		}

		unblocked->status = PCB::READY;
		Scheduler::put(unblocked);
	}
	unlock;
#endif
}
