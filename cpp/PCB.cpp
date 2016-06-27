/*
 * PCB.cpp
 *
 *  Created on: Aug 21, 2015
 *      Author: Vladimir
 */

#include "PCB.h"
#include "WaitingQ.h"
#include "SleepQ.h"
#include "kernel.h"
#include "Schedule.h"
#include <dos.h>

PCB** PCB::PCBS=new PCB*[MAX_PCB_NUM];
PCB* PCB::running=0;
PCB* PCB::idle_pcb=0;
PCB* PCB::kernel=0;
int PCB::in_kernel_mode=0;
int PCB::indexCounter=0;

void PCB::sleep(Time timeToSleep){
	if (timeToSleep==0) Scheduler::put(PCB::running);
	else	SleepQ::sleep(timeToSleep);
	running=Scheduler::get();
}

void PCB::dispatch(){
	Scheduler::put(running);
	running=Scheduler::get();
	kernel::timeCounter=0;
}


void PCB::releaseWaitingThreads(){
		myWaitingQueue->releaseWaitingThreads();
		running->state=FINISHED;
		running=Scheduler::get();
		kernel::timeCounter=0;
}

void PCB::putInWaitingQueue(){
	if (state==READY || state==BLOCKED){
		myWaitingQueue->blockRunningPCB();
		kernel::timeCounter=0;
	}
	else
		dispatch();

}


void PCB::start(){
	if (state==NEW)
	{
		state=READY;
		Scheduler::put(this);
		dispatch();//
	}
}

ID PCB::pcb_create(Thread *newThread,StackSize stackSize,Time timeSlice){
	PCB *myPCB=new PCB(newThread,stackSize,timeSlice);
	return myPCB->myID;
}

PCB::~PCB(){
	if (stack!=0) {delete stack; stack=0;}
	if (myWaitingQueue!=0) {delete myWaitingQueue; myWaitingQueue=0;}
	if (myID>=0) PCBS[myID]=0;
	unlimited_time=myID=time=ss=sp=0;
}

PCB::PCB(){
	stack=0;
	myID=MAIN_ID;
	unlimited_time=0;
	//unlimited_time=1;
	time=defaultTimeSlice;
	myWaitingQueue=new WaitingQ();
}

PCB::PCB(int mode,StackSize size_of_stack){
	time=1;
	unlimited_time=0;
	size_of_stack/=sizeof(unsigned);
	stack=new unsigned[size_of_stack];
	stack[size_of_stack-3]=0x200;
	myWaitingQueue=new WaitingQ();
	state=NEW;

	if (mode==IDLE_STACK_INITIALIZATION_MODE)
	{
	#ifndef BCC_BLOCK_IGNORE
		stack[size_of_stack-4]=FP_SEG(kernel::idle);
		stack[size_of_stack-5]=FP_OFF(kernel::idle);
	#endif
		myID=IDLE_ID;
	}
	else
	{
	#ifndef BCC_BLOCK_IGNORE
		stack[size_of_stack-4]=FP_SEG(kernel::systemService);//kernel
		stack[size_of_stack-5]=FP_OFF(kernel::systemService);//kernel
	#endif

		myID=KERNEL_ID;
	}
#ifndef BCC_BLOCK_IGNORE
	ss=FP_SEG(stack+size_of_stack-14);
	sp=FP_OFF(stack+size_of_stack-14);
#endif
	stack[size_of_stack-14]=sp;
}

PCB::PCB(Thread *newThread,StackSize size_of_stack,Time timeSlice){
	if (timeSlice==0) unlimited_time=1;
	else unlimited_time=0;

	time=timeSlice;
	myID=indexCounter++;
	size_of_stack/=sizeof(unsigned);
	stack=new unsigned[size_of_stack];
	myWaitingQueue=new WaitingQ();
	state=NEW;

#ifndef BCC_BLOCK_IGNORE
	stack[size_of_stack-1]=FP_SEG(newThread);
	stack[size_of_stack-2]=FP_OFF(newThread); //-3,-4 su callback (Nebitno, nikad se ne vracamo iz wrapper)
	stack[size_of_stack-5]=0x200; //I=1;
	stack[size_of_stack-6]=FP_SEG(Thread::wrapper);
	stack[size_of_stack-7]=FP_OFF(Thread::wrapper);
	ss=FP_SEG(stack+size_of_stack-16);
	sp=FP_OFF(stack+size_of_stack-16);
#endif
	stack[size_of_stack-16]=sp;
	PCBS[myID]=this;
}
