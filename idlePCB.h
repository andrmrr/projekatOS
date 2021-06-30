/*
 * idleThread.h
 *
 *  Created on: Jun 24, 2021
 *      Author: OS1
 */

#ifndef IDLEPCB_H_
#define IDLEPCB_H_

#include "pcb.h"

void idle();


//kreiranje idle niti
class IdlePCB : public PCB{
public:

	~IdlePCB();


	void waitToComplete() {}
	static PCB* createIdlePCB();

	 ID getId() {return 1;}

protected:
	void deblock(){}

private:
	IdlePCB();

};


#endif /* IDLEPCB_H_ */
