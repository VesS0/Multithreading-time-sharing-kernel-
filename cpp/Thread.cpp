/*
 * Thread.cpp
 *
 *  Created on: Aug 20, 2015
 *      Author: Vladimir
 */

#include "Thread.h"
#include "IDStruct.h"
#include <dos.h>

void dispatch(){
		IDStruct *help=new IDStruct();
		help->callerID=NO_CALLER;
		help->functionCalled=PCB_DISPATCH;

		unsigned segment,offs;
	#ifndef BCC_BLOCK_IGNORE
		offs=FP_OFF(help);
		segment=FP_SEG(help);

		asm{
			mov cx,offs
			mov bx,segment
			int ENTER_KERNEL
		}
	#endif

		delete help;
}

void Thread::start(){
	IDStruct *help=new IDStruct();
	help->callerID=myPCB;
	help->functionCalled=PCB_START;

	unsigned segment,offs;
#ifndef BCC_BLOCK_IGNORE
	offs=FP_OFF(help);
	segment=FP_SEG(help);

	asm{
		mov cx,offs
		mov bx,segment
		int ENTER_KERNEL
	}
#endif

	delete help;
}

void Thread::waitToComplete(){
	IDStruct *help=new IDStruct();
	help->callerID=myPCB;
	help->functionCalled=PCB_WAIT_TO_COMPLETE;


	unsigned segment,offs;
#ifndef BCC_BLOCK_IGNORE
	offs=FP_OFF(help);
	segment=FP_SEG(help);

	asm{
		mov cx,offs
		mov bx,segment
		int ENTER_KERNEL
	}
#endif

	delete help;
}


Thread::~Thread(){
	IDStruct *help=new IDStruct();
	help->callerID=myPCB;
	help->functionCalled=PCB_DELETE;

	unsigned segment,offs;
#ifndef BCC_BLOCK_IGNORE
	offs=FP_OFF(help);
	segment=FP_SEG(help);

	asm{
		mov cx,offs
		mov bx,segment
		int ENTER_KERNEL
	}
#endif

	delete help;
}

void Thread::sleep(Time timeToSleep){
	IDStruct *help=new IDStruct();
	help->functionCalled=PCB_SLEEP;
	help->arg[0]=&timeToSleep;

		unsigned segment,offs;
	#ifndef BCC_BLOCK_IGNORE
		offs=FP_OFF(help);
		segment=FP_SEG(help);

		asm{
			mov cx,offs
			mov bx,segment
			int ENTER_KERNEL
		}
	#endif

		delete help;
}

Thread::Thread(StackSize stackSize, Time timeSlice)
{
		IDStruct *help=new IDStruct();
		help->callerID=NO_CALLER;
		help->functionCalled=PCB_CREATE;
		help->arg[0]=this;
		help->arg[1]=&stackSize;
		help->arg[2]=&timeSlice;

		unsigned segment,offs;
	#ifndef BCC_BLOCK_IGNORE
		offs=FP_OFF(help);
		segment=FP_SEG(help);

		asm{
			mov cx,offs
			mov bx,segment
			int ENTER_KERNEL
		}
	#endif

		myPCB=help->callerID;
		delete help;
}

void Thread::wrapper(Thread* running)
{
	running->run();

	IDStruct *help=new IDStruct();
	help->callerID=running->myPCB;
	help->functionCalled=PCB_RELEASE_WAITING;

	unsigned segment,offs;
#ifndef BCC_BLOCK_IGNORE
	offs=FP_OFF(help);
	segment=FP_SEG(help);

	asm{
		mov cx,offs
		mov bx,segment
		int ENTER_KERNEL
	}
#endif

}
