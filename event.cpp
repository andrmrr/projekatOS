/*
 * event.cpp
 *
 *  Created on: Jun 30, 2021
 *      Author: OS1
 */

#include "event.h"
#include "krnEv.h"
#include "IVTEntry.h"
#include "general.h"

Event::Event(IVTNo ivtNo){
#ifndef BCC_BLOCK_IGNORE
	lock;
	myImpl = new KernelEv(ivtNo, this);
	unlock;
#endif
	IVTEntry::ivteArr[ivtNo]->setMyKernelEv(myImpl);
}

Event::~Event(){
	if(myImpl){
#ifndef BCC_BLOCK_IGNORE
	lock;
	delete myImpl;
	unlock;
#endif
	}
	//da li treba da se ovde obrise i ivtentry
}

void Event::wait(){
	if(myImpl){
		myImpl->wait();
	}
}

void Event::signal(){
	if(myImpl){
		myImpl->signal();
	}
}
