/*
 * kernlelSem.h
 *
 *  Created on: Jun 28, 2021
 *      Author: OS1
 */

#ifndef KERNELSEM_H_
#define KERNELSEM_H_

class KernelSem {
public:
	KernelSem();
	virtual ~KernemSem();

	virtual int wait(Time maxTimeToWait);
	virtual void signal();

	int val() const;


private:

};



#endif /* KERNELSEM_H_ */
