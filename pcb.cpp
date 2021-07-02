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
		//syncPrintf("Izlazimo iz waitToComplete-a\n");
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
	unlock;
	dispatch();
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
	sig = 0;
#ifndef BCC_BLOCK_IGNORE
	lock;
	if(stackSize > 65535) stackSize = 65535;
	stackSize /= sizeof(unsigned);
	if(stackSize < defaultStackSize/sizeof(unsigned)) stackSize = defaultStackSize/sizeof(unsigned);
	size = stackSize;
	stack = new unsigned[stackSize];
	unlock;
	stack[stackSize - 1] = 0x200; //PSW
	stack[stackSize - 2] = FP_SEG(wrapper); //CS
	stack[stackSize - 3] = FP_OFF(wrapper); //IP
	//stack[1020-1012] = {AX, BX, CX, DX, ES, DS, SI, DI, BP}
	ss = FP_SEG(stack + stackSize - 12);
	sp = FP_OFF(stack + stackSize - 12);
	bp = sp;
#endif
}

PCB::PCB(){
	quantum = defaultTimeSlice;
	sig = 0;
	size = 0;
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

//	FORK metode

ID PCB::fork(){
#ifndef BCC_BLOCK_IGNORE
    lock;
	//kloniranje niti
	Thread* newthr = running->myThread->clone();
	syncPrintf("ID deteta: %d\n", newthr->myPCB->id);
	if(newthr == 0){
		unlock;
		return -1;
	}
	PCB* newpcb = newthr->myPCB;
	PCB* temp = (PCB*)running;

	newpcb->start();

	unsigned* runningBasePointer = (unsigned*)(MK_FP(temp->ss, temp->bp));
	unsigned* runningStackPointer = (unsigned*)(MK_FP(temp->ss, temp->sp));
	unsigned* newStackPointer = newpcb->stack + (runningStackPointer - temp->stack);
	//postavljamo sp  i ss deteta
	newpcb->ss = FP_SEG(newStackPointer);
	newpcb->sp = FP_OFF(newStackPointer);
	//unsigned* newBasePointer = (unsigned*)(MK_FP(newpcb->ss, newpcb->bp));

	//kopiranje steka
	int i;
	unsigned* rbp = runningBasePointer, *nbp = newStackPointer + (runningBasePointer - runningStackPointer);
	//postavljamo bp deteta
	newpcb->bp = FP_OFF(nbp);
	syncPrintf("rbp: %d\n", (temp->stack + temp->size-1) - runningStackPointer);
	syncPrintf("nbp: %d\n", (newpcb->stack + newpcb->size-1) - newStackPointer);
	for(i = 0; (temp->stack + temp->size) != (runningStackPointer + i); i++){
		*(newStackPointer + i) = *(runningStackPointer + i);
		syncPrintf("i: %d\n", i);
		//provera za bp
		//if((runningStackPointer + i) == rbp){
			//rbp =
		//}
	}


	//sredjujemo base pointere na steku
	unsigned *rbp1, *rbp2, *nbp1, *nbp2;
	nbp1 = newStackPointer + (runningBasePointer - runningStackPointer);
	newpcb->bp = FP_OFF(nbp1);
	rbp1 = runningBasePointer;
	rbp2 = (unsigned*)(*(rbp1+1)*65536 + *(rbp1));
	cout << "novi stek: " << (newpcb->stack + newpcb->size) << endl;
	cout << "stari stek: " << (temp->stack + temp->size)<< endl;
	cout << (unsigned long)(*(rbp1+1)* 65536 + *(rbp1)) << endl;
	cout << "rbp1: " << rbp1 << " *rbp1: " << *rbp1 << " *(rbp1+1): " << *(rbp1 +1) << endl;
	cout << "rbp2: " << rbp2 << endl;
	while(rbp2 < temp->stack + temp->size && temp->stack < rbp2){
		cout << "nbp1: " << nbp1 << endl;
		*nbp1 = (unsigned)nbp1 + (rbp2 - rbp1);
		nbp2 = (unsigned*)*nbp1;
		rbp1 = rbp2;
		rbp2 = (unsigned*)*rbp2;
		nbp1 = nbp2;
		cout << "rbp1: " << rbp1 << endl;
		cout << "rbp2: " << rbp2 << endl;
	}



	if((PCB*)running == temp){
		temp->children.add(newpcb->id);
		unlock;
		syncPrintf("Povratni ID deteta newthr: %d\n", newthr->myPCB->id);
		syncPrintf("Povratni ID deteta: %d\n", newpcb->id);
		return newpcb->id;
	}
	else{
		return 0;
	}

#endif
}


void PCB::exit(){
#ifndef BCC_BLOCK_IGNORE
	lock;
	//cout << "Zavrsili nit " << running->getRunningId() << endl;
	running->status = FINISHED;
	unfinished --;
	running->deblock();
	unlock;
	dispatch();
#endif
}

void PCB::waitForForkChildren(){
	PCB* temp = (PCB*)running;
	temp->children.goToFirst();
	while(temp->children.isCurrent()){
		ID t = temp->children.getCurrent();
		Thread* thr = getThreadById(t);
		thr->waitToComplete();
		temp->children.goToNext();
	}
}
