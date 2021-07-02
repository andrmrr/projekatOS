/*
 * copyThrd.h
 *
 *  Created on: Jul 2, 2021
 *      Author: OS1
 */

#ifndef COPYTHRD_H_
#define COPYTHRD_H_

#include "thread.h"

class CopyThread : public Thread {
public:
	PCB* parent, *child;
	CopyThread(PCB* p, PCB *c);
	~CopyThread(){this->waitToComplete();}
protected:
	virtual void run();
};


#endif /* COPYTHRD_H_ */
