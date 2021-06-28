/*
 * timer.h
 *
 *  Created on: Jun 23, 2021
 *      Author: OS1
 */

#ifndef TIMER_H_
#define TIMER_H_

//globalni brojac za tajmer

//pomocne promenljive za prekid tajmera
extern volatile unsigned tsp;
extern volatile unsigned tss;
extern volatile unsigned tbp;

extern volatile unsigned counter;
extern volatile int context_switch_on_demand;

//globalna funkcija koja mora da se uvek pozove
void tick();

// stara prekidna rutina
//definisana u system.h
extern unsigned oldTimerOFF, oldTimerSEG;

// deklaracija nove prekidne rutine
void interrupt timer();


//void inic();
//void restore();
//prebaceno u System





#endif /* TIMER_H_ */
