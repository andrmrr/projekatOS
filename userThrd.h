/*
 * userThread.h
 *
 *  Created on: Jun 24, 2021
 *      Author: OS1
 */

#ifndef USERTHRD_H_
#define USERTHRD_H_

#include "thread.h"
#include <IOSTREAM.H>

//potpis korisnickog main-a


class UserThread : public Thread{
	int argc;
	char **argv;
	int retMain;
	static int userMain(int argc, char* argv[]);

public:

	UserThread(int c, char** v, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	~UserThread();
	int getRet();

protected:
	void run();



};



#endif /* USERTHRD_H_ */
