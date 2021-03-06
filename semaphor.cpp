/*
 * semaphor.cpp
 *
 *  Created on: Jun 27, 2021
 *      Author: OS1
 */

#include "semaphor.h"
#include "krnSem.h"
#include <LIMITS.H>
#include "general.h"

Semaphore::Semaphore(int init) {
#ifndef BCC_BLOCK_IGNORE
	lock;
	myImpl = new KernelSem(init);
	//myImpl = KernelSem::createKernelSem(this, init);
	//cout << "myImpl: " << myImpl << ";  val: " << myImpl->val() << endl;
	unlock;
#endif
}

Semaphore::~Semaphore (){
	//cout << "Usli u destruktor Semafora\n";
#ifndef BCC_BLOCK_IGNORE
		lock;
	if(myImpl){
		delete myImpl;
		myImpl = 0;
	}
		unlock;
#endif
}

int Semaphore::wait (Time maxTimeToWait){
	if(myImpl){
		//cout << "Wait u Semaphore-u\n";
		return myImpl->wait(maxTimeToWait);
	}
	return -1;
}

void Semaphore::signal(){
	if(myImpl){
		myImpl->signal();
	}
}

int Semaphore::val () const{
	if(myImpl){
			return myImpl->val();
		}
	return INT_MAX;
}
