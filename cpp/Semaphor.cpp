/*
 * Semaphor.cpp
 *
 *  Created on: Aug 25, 2015
 *      Author: Vladimir
 */

#include "Semaphor.h"
#include "IDStruct.h"
#include <dos.h>

Semaphore::Semaphore (int init)
{
	IDStruct *help=new IDStruct();
	help->functionCalled=SEM_CREATE;
	help->arg[0]=&init;

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
	myImpl=help->callerID;

	delete help;
}



Semaphore::~Semaphore() {
	IDStruct *help=new IDStruct();
	help->callerID=myImpl;
	help->functionCalled=SEM_DELETE;

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

int Semaphore::wait(int blockNo)
{
	IDStruct *help=new IDStruct();
	help->callerID=myImpl;
	help->functionCalled=SEM_WAIT;
	help->arg[0]=&blockNo;

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
	int ret=help->callerID;
	delete help;
	return ret;
}
void Semaphore::signal(){
	IDStruct *help=new IDStruct();
	help->callerID=myImpl;
	help->functionCalled=SEM_SIGNAL;

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

int Semaphore::val () const
{
	IDStruct *help=new IDStruct();
	help->callerID=myImpl;
	help->functionCalled=SEM_VALUE;
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

	int value=*((int*)help->arg[0]);
	delete help;
	return value;
}

