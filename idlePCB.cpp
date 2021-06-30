/*
 * idlePCB.cpp
 *
 *  Created on: Jun 24, 2021
 *      Author: OS1
 */

#include "idlePCB.h"
#include <DOS.H>


void idle() {
	while(1){
		int k, j;
		cout<< "IDLE ";
		for ( k = 0; k<10000; ++k)
			for ( j = 0; j <3000; ++j);
	}
}

IdlePCB::IdlePCB() : PCB(0, defaultStackSize, 1) {
		status = PCB::IDLE;
		#ifndef BCC_BLOCK_IGNORE
		stack[1022] = FP_SEG(idle);
		stack[1021] = FP_OFF(idle);
		#endif
	}

PCB* IdlePCB::createIdlePCB(){
		PCB* newPCB = new IdlePCB();
		if(newPCB == 0) return 0;

		newPCB->id = PCB::pcbList.insert(newPCB);
		if(newPCB->id == -1) return 0;
		return newPCB;
}


IdlePCB::~IdlePCB(){
		cout << "Brisemo IDLE\n";
	}
