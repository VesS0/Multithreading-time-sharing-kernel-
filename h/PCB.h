/*
 * PCB.h
 *
 *  Created on: Aug 21, 2015
 *      Author: Vladimir
 */

#ifndef _PCB_H
#define _PCB_H

//#include "SleepQ.h"
//#include "FreeInd.h"

#include "Thread.h"

class WaitingQ;

class PCB{

public:
	static PCB *running;
	static PCB **PCBS;
	static PCB *kernel;
	static PCB *idle_pcb;
	static int in_kernel_mode;
	static int indexCounter;

	static void dispatch();
	static void sleep(Time);
	static ID pcb_create(Thread*,StackSize,Time);

	STATE state;

	void start();
	void releaseWaitingThreads();
	void putInWaitingQueue();

	~PCB();
	PCB();
	PCB(Thread *newThread,StackSize size_of_stack,Time timeSlice);
	PCB(int i,StackSize size_of_stack);

private:
	int unlimited_time;
	ID myID;
	Time time;
	unsigned ss,sp;
	unsigned  *stack;
	WaitingQ *myWaitingQueue;

	friend class kernel;
};


#endif
