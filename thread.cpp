/*
 * Thread.cpp
 *
 *  Created on: Jun 22, 2021
 *      Author: OS1
 */
#include "thread.h"
#include "pcb.h"
#include "general.h"

Thread::Thread(StackSize stackSize, Time timeSlice){
#ifndef BCC_BLOCK_IGNORE
	lock;
	myPCB = PCB::createPCB(this, stackSize, timeSlice);
	unlock;
#endif
}

void Thread::start(){
	if(myPCB){
		myPCB->start();
	}
}

void Thread::waitToComplete(){
	if(myPCB){
		//cout << "Thread waitToComplete\n";
		myPCB->waitToComplete();
	}
}

Thread::~Thread() {
	this->waitToComplete();//mozda ne radi posao
	//cout << "Thread destruktor\n";
#ifndef BCC_BLOCK_IGNORE
	lock;
	if(myPCB){
		delete myPCB;
		myPCB = 0;
	}
	unlock;
#endif
}

ID Thread::getId() {
	if(myPCB){
		return myPCB->getId();
	}
	else{
		return -1;
	}
}

ID Thread::getRunningId(){
	return PCB::getRunningId();
}

Thread
* Thread::getThreadById(ID id){
	return PCB::getThreadById(id);
}


//FORK METODE
Thread* Thread::clone() const{
	syncPrintf("Pozvana je clone metoda osnovne klase Thread!\n");
	return 0;
}

ID Thread::fork(){
	return PCB::fork();
}

void Thread::exit(){
	PCB::exit();
}

void Thread::waitForForkChildren(){
	PCB::waitForForkChildren();
}
