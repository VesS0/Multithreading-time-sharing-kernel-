/*
 * KerSem.cpp
 *
 *  Created on: Aug 25, 2015
 *      Author: Vladimir
 */

#include "KerSem.h"
#include "WaitingQ.h"

KernelSem *kersem[MAX_SEM_NUM];
KernelSem** KernelSem::SEMS=kersem;//*[MAX_NUM_SEMS];

int KernelSem::indexCounter=0;


ID KernelSem::sem_create(int value)
{
	KernelSem *mySem=new KernelSem(value);
	SEMS[mySem->myID]=mySem;
	return mySem->myID;
}

const int* KernelSem::val()const
{
	return &myValue;
}

int KernelSem::wait(int blockNo){
	if (blockNo==0){
	if (myValue--<=0)
		myBlockedQueue->blockRunningPCB();
	return 1;
	}
	else
	{
		if(myValue>0){
			myValue--;
			return 1;
		}
	}
	return 0;
}
void KernelSem::signal(){
	if (myValue++<0)
		myBlockedQueue->unblockOne();
}
KernelSem::KernelSem(int init) {
	if (init<0) init=0;
	myValue=init;
	myBlockedQueue=new WaitingQ();
	myID=indexCounter++;
}


KernelSem::~KernelSem() {
	while (myValue<0) signal();
	if (myBlockedQueue!=0) {delete myBlockedQueue;	myBlockedQueue=0;}
	if (myID>=0) SEMS[myID]=0;
}

