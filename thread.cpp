/*
 * Thread.cpp
 *
 *  Created on: Jun 22, 2021
 *      Author: OS1
 */
#include "thread.h"
#include "pcb.h"

Thread::Thread(StackSize stackSize, Time timeSlice){
	myPCB = PCB::createPCB(this, stackSize, timeSlice);
}

void Thread::start(){
	if(myPCB){
		myPCB->start();
	}
}

void Thread::waitToComplete(){
	if(myPCB){
		myPCB->waitToComplete();
	}
}

Thread::~Thread() {
	if(myPCB){
		cout << "Thread destruktor\n";
		delete myPCB;
		myPCB = 0;
	}
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

Thread * Thread::getThreadById(ID id){
	return PCB::getThreadById(id);
}
