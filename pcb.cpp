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
	cout <<"Metoda start\n" << endl;
	if(this->status == INITIALIZED){
		this->status = READY;
		Scheduler::put(this);
		unfinished++;
	}
	unlock;
#endif
}

void  PCB::waitToComplete(){
	//ubacujemo tekucu nit u listu koje cekaju this nit da se zavrsi
	cout << "Pokusaj waitToComplete-a za nit " << this->id<< endl;
	if(this != running && this->status != FINISHED && this->status != IDLE && this->status != INITIALIZED){
#ifndef BCC_BLOCK_IGNORE
	lock;
		cout << "UPADAMO U WAITTOCOMPLETE BATOO\n";
		running->status = BLOCKED;
		this->waitToCompleteList.add((PCB*)running);
	unlock;
#endif
		dispatch();
	}
}

PCB::~PCB(){
#ifndef BCC_BLOCK_IGNORE
	lock;
	cout << "Ulazimo u PCB destruktor\n";
	cout << "Remove vraca: " << pcbList.remove(this->id) << endl;
	if(stack){
		delete[] stack;
		stack = 0;
	}
	unlock;
#endif
}

ID PCB::getId(){
	return pcbList.getID(this->myThread);
}

ID PCB::getRunningId(){
	if(running){
		return pcbList.getID(running->myThread);
	}
	return -1;
}

Thread * PCB::getThreadById(ID id){
	return pcbList.getThreadById(id);
}



void PCB::wrapper(){
	//cout <<"Thread: " << (void*)running->myThread << endl;
	running->myThread->run();

#ifndef BCC_BLOCK_IGNORE
	lock;
	cout << "Zavrsili nit " << running->getRunningId() << endl;
	unlock;
	running->status = FINISHED;
	unfinished --;
	running->deblock();
	dispatch();
#endif
}

PCB* PCB::createPCB(Thread* mt, StackSize stackSize, Time timeSlice){
	PCB* newPCB = new PCB(mt, stackSize, timeSlice);
	if(newPCB == 0) return 0;

	newPCB->id = pcbList.insert(newPCB);
	if(newPCB->id == -1) return 0;
	return newPCB;
}

PCB* PCB::createPCB(){
	PCB* newPCB = new PCB();
	if(newPCB == 0) return 0;

	newPCB->id = pcbList.insert(newPCB);
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
	stack = new unsigned[stackSize];
	unlock;
	stack[1023] = 0x200; //PSW
	stack[1022] = FP_SEG(PCB::wrapper); //CS
	stack[1021] = FP_OFF(PCB::wrapper); //IP
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
		waitToCompleteList.goToFirst();
		while(waitToCompleteList.isCurrent()){
			PCB* temp = waitToCompleteList.getCurrent();
#ifndef BCC_BLOCK_IGNORE
			lock;
			temp->status = READY;
			Scheduler::put(temp);
			unlock;
#endif
			waitToCompleteList.goToNext();
		}
}
