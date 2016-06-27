/*
 * Thread.h
 *
 *  Created on: Aug 20, 2015
 *      Author: Vladimir
 */

// File: thread.h
#ifndef _THREAD_H
#define _THREAD_H

#include "Const.h"


class PCB; // Kernel's implementation of a user's thread

class Thread {

public:

	void start();
	void waitToComplete();
	virtual ~Thread();
	static void sleep(Time timeToSleep);

protected:

	friend class PCB;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}

private:
	static void wrapper(Thread* running);
	ID myPCB;
};

void dispatch();

#endif
