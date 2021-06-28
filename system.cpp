/*
 * system.cpp
 *
 *  Created on: Jun 24, 2021
 *      Author: OS1
 */


#include "pcb.h"
#include "system.h"
#include <DOS.H>
#include "timer.h"
#include "userThrd.h"
#include "idlePCB.h"
#include "general.h"

// podaci iz timer.h
unsigned oldTimerOFF, oldTimerSEG;

//globalne promenljive
Thread* userMainThread;
PCB* idlePCB;
PCB* mainPCB;


System::System(int argc, char** argv) {

		// postavlja novu prekidnu rutinu tajmera
#ifndef BCC_BLOCK_IGNORE
		lock;
			asm{

				push es
				push ax

				mov ax,0
				mov es,ax // es = 0

				// pamti staru rutinu
				mov ax, word ptr es:0022h
				mov word ptr oldTimerSEG, ax
				mov ax, word ptr es:0020h
				mov word ptr oldTimerOFF, ax


				// postavlja novu rutinu
				mov word ptr es:0022h, seg timer
				mov word ptr es:0020h, offset timer


				// postavlja staru rutinu na int 60h
				mov ax, oldTimerSEG
				mov word ptr es:0182h, ax
				mov ax, oldTimerOFF
				mov word ptr es:0180h, ax

				pop ax
				pop es

			}

			//kreira main PCB
			mainPCB = PCB::createPCB();
			PCB::running = mainPCB;

			//kreira "zaludnu" nit
			idlePCB = IdlePCB::createIdlePCB();
			cout << "Idle status: " << idlePCB->status << endl;

			//inicijalizuje main kao prvu nit
			userMainThread = new UserThread(argc, argv);
			//userMainThread->start();

			//inicijalizuje context_switch_on_demand
			context_switch_on_demand = 0;
			//counter = PCB::running->quantum;
			cout << "counter: " << counter << endl;
			cout << "unfinished: " << PCB::unfinished << endl;
	unlock;
#endif
	}

System::~System(){
	cout << "System destruktor\n";
// vraca staru prekidnu rutinu
#ifndef BCC_BLOCK_IGNORE
	lock;
	asm {

		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es

	}

	//brise idlenit
	cout << "Brisemo idlePCB\n";
	delete idlePCB;

	//brise Usermain nit
	cout <<"Brisemo userMainThread\n";
	delete userMainThread;

	//brise main PCB
	cout <<"Brisemo mainPCB\n";
	delete mainPCB;

	unlock;
#endif;
}
