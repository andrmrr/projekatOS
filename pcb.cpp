/*
 * pcb.cpp
 *
 *  Created on: Jun 22, 2021
 *      Author: OS1
 */

#include <dos.h>
#include <IOSTREAM.H>
#include "general.h"
#include "SCHEDULE.H"
#include "timer.h"
#include "pcb.h"
#include "PCBList.h"


PCBList PCB::pcbList;
volatile PCB* PCB::running = 0;
unsigned PCB::unfinished = 0;


void PCB::start(){
#ifndef BCC_BLOCK_IGNORE
	lock;
	//cout <<"Metoda start\n" << endl;
	if(this->status == INITIALIZED){
		this->status = READY;
		Scheduler::put(this);
		unfinished++;
	}
	unlock;
#endif
}

void  PCB::waitToComplete(){
	//cout << "Pokusaj waitToComplete-a za nit " << this->id<< endl;
#ifndef BCC_BLOCK_IGNORE
	lock;
	if(this != running && this->status != FINISHED && this->status != IDLE && this->status != INITIALIZED){
		//cout << "UPADAMO U WAITTOCOMPLETE BATOO\n";
		running->status = BLOCKED;
		this->waitToCompleteList.add((PCB*)running);
		dispatch();
	}
	unlock;
#endif
}

PCB::~PCB(){
#ifndef BCC_BLOCK_IGNORE
	lock;
	//cout << "Ulazimo u PCB destruktor\n";
	//cout << "Remove vraca: " << pcbList.remove(this->id) << endl;
	pcbList.remove(this->id);
	if(stack){
		delete[] stack;
		stack = 0;
	}
	unlock;
#endif
}

ID PCB::getId(){
	return this->id;
	//return pcbList.getID(this->myThread);
}

ID PCB::getRunningId(){
	return running->id;
	//return pcbList.getID(running->myThread);
}

Thread * PCB::getThreadById(ID id){
	return pcbList.getThreadById(id);
}



void PCB::wrapper(){
	//cout <<"Thread: " << (void*)running->myThread << endl;
	running->myThread->run();

#ifndef BCC_BLOCK_IGNORE
	lock;
	//cout << "Zavrsili nit " << running->getRunningId() << endl;
	running->status = FINISHED;
	unfinished --;
	running->deblock();
	dispatch();
	unlock;
#endif
}

PCB* PCB::createPCB(Thread* mt, StackSize stackSize, Time timeSlice){
	PCB* newPCB;
#ifndef BCC_BLOCK_IGNORE
	lock;
	newPCB = new PCB(mt, stackSize, timeSlice);
	unlock;
#endif
	if(newPCB == 0) return 0;
#ifndef BCC_BLOCK_IGNORE
	lock;
	newPCB->id = pcbList.insert(newPCB);
	unlock;
#endif
	if(newPCB->id == -1) return 0;
	return newPCB;
}

PCB* PCB::createPCB(){
	PCB* newPCB;
#ifndef BCC_BLOCK_IGNORE
	lock;
	newPCB = new PCB();
	unlock;
#endif
	if(newPCB == 0) return 0;
#ifndef BCC_BLOCK_IGNORE
	lock;
	newPCB->id = pcbList.insert(newPCB);
	unlock;
#endif
	if(newPCB->id == -1) return 0;
	return newPCB;
}

PCB::PCB(Thread* mt, StackSize stackSize, Time timeSlice){
	quantum = timeSlice;
	myThread = mt;
	status = INITIALIZED;

#ifndef BCC_BLOCK_IGNORE
	lock;
	stackSize /= sizeof(unsigned);
	if(stackSize > 65535) stackSize = 65535;
	if(stackSize < defaultStackSize/sizeof(unsigned)) stackSize = defaultStackSize/sizeof(unsigned);
	stack = new unsigned[stackSize];
	unlock;
	stack[1023] = 0x200; //PSW
	stack[1022] = FP_SEG(wrapper); //CS
	stack[1021] = FP_OFF(wrapper); //IP
	//stack[1020-1012] = {AX, BX, CX, DX, ES, DS, SI, DI, BP}
	ss = FP_SEG(stack + 1012);
	sp = FP_OFF(stack + 1012);
	bp = sp;
#endif
}

PCB::PCB(){
	quantum = defaultTimeSlice;
	stack = 0;
	myThread = 0;
	status = READY;
	ss = 0;
	sp = 0;
	bp = 0;
}

void PCB::deblock(){
#ifndef BCC_BLOCK_IGNORE
		lock;
		waitToCompleteList.goToFirst();
		while(waitToCompleteList.isCurrent()){
			PCB* temp = waitToCompleteList.getCurrent();
			temp->status = READY;
			Scheduler::put(temp);
			waitToCompleteList.goToNext();
		}
		unlock;
#endif
}
