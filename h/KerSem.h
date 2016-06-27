/*
 * KerSem.h
 *
 *  Created on: Aug 25, 2015
 *      Author: Vladimir
 */

#ifndef KERSEM_H_
#define KERSEM_H_

#include "Const.h"

class WaitingQ;

class KernelSem {

public:
	static KernelSem **SEMS;
	static int indexCounter;
	static ID sem_create(int value=1);

	int wait (int blockNo);
	void signal();
 	const int* val ()const; // Returns the current value of the semaphore
	KernelSem(int value=1);
	virtual ~KernelSem();

private:
	int myValue;
	int myID;
	WaitingQ *myBlockedQueue;

};

#endif /* KERSEM_H_ */
