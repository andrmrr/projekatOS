// File: semaphor.h
#ifndef _semaphor_h_
#define _semaphor_h_

typedef unsigned int Time;

class KernelSem;

class Semaphore {

public:
	Semaphore (int init=1);
	 ~Semaphore ();

	int wait (Time maxTimeToWait);
	void signal();

	int val () const; // Returns the current value of the semaphore

private:
	KernelSem* myImpl;

};
#endif
