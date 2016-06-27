/*
 * event.cpp
 *
 *  Created on: Aug 28, 2015
 *      Author: Vladimir
 */

#include "event.h"
#include "IDStruct.h"
#include <dos.h>

Event::Event(IVTNo ivtNo) {
	IDStruct *help=new IDStruct();
	help->functionCalled=EVE_CREATE;
	help->arg[0]=&ivtNo;

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

Event::~Event() {
	IDStruct *help=new IDStruct();
	help->callerID=myImpl;
	help->functionCalled=EVE_DELETE;

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

void Event::wait()
{
	IDStruct *help=new IDStruct();
	help->callerID=myImpl;
	help->functionCalled=EVE_WAIT;

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

void Event::signal()
{
	IDStruct *help=new IDStruct();
	help->callerID=myImpl;
	help->functionCalled=EVE_SIGNAL;

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
