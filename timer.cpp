/*
 * timer.cpp
 *
 *  Created on: Jun 24, 2021
 *      Author: OS1
 */

#include "timer.h"
#include <DOS.H>
#include "general.h"
#include "SCHEDULE.H"
#include "pcb.h"
#include <IOSTREAM.H>


//pomocne promenljive za prekid tajmera
volatile unsigned tsp;
volatile unsigned tss;
volatile unsigned tbp;

volatile unsigned counter;
volatile int context_switch_on_demand;

void tick(){}

void interrupt timer(){
	if(!context_switch_on_demand) {
		if(PCB::running->quantum != 0){
			counter--;
		}
		tick();
#ifndef BCC_BLOCK_IGNORE
		asm int 60h;
#endif
	}

	if((counter == 0 && PCB::running->quantum != 0) || context_switch_on_demand){

		//cuvanje starog konteksta
#ifndef BCC_BLOCK_IGNORE
		asm{
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
#endif
		//if(PCB::running){
		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		//ako je spremna trenutna nit se ubacuje u Scheduler
		if(PCB::running->status == PCB::READY){
			Scheduler::put((PCB*)PCB::running);
			cout << "Stavljamo nit: " << PCB::running->id <<" u scheduler\n";
		}
		//}

		cout << "Unfinished: " << PCB::unfinished << endl;

		//uzimamo novu nit za izvrsavanje iz Schedulera
		PCB::running = Scheduler::get();
		cout << "Running ID: " << PCB::running->id << endl << endl;
		if(PCB::running  == 0){
			//palimo idle nit
			PCB::running = idlePCB;
		}

		//cout << "Status: " << PCB::running->status << endl;

		//restauriramo novi kontekst
		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;
		counter = PCB::running->quantum;
#ifndef BCC_BLOCK_IGNORE
		asm{
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
#endif

		context_switch_on_demand = 0;
	}
}

void dispatch(){
#ifndef BCC_BLOCK_IGNORE
	lock;
	context_switch_on_demand = 1;
	timer();
	unlock;
#endif
}
