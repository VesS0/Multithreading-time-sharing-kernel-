/*
 * IVTEntry.cpp
 *
 *  Created on: Aug 28, 2015
 *      Author: Vladimir
 */

#include "IVTEntry.h"
#include "KernelEv.h"
#include "IDStruct.h"
#include "EventQ.h"
#include "PCB.h"
#include <dos.h>
#include <iostream.h>

IVTEntry *IVt[NUM_OF_ENTRIES];
IVTEntry ** IVTEntry::IVTEntries=IVt;

EventQ* IVTEntry::myWaitingEvents=new EventQ();

void IVTEntry::setEvent(KernelEv *myEve)
{
	myEvent=myEve;
}

void IVTEntry::oldRoute()
{
	old_interrupt_No();
}

IVTEntry::IVTEntry(IVTNo No,ptrInterrupt new_interrupt_No) {
	myNo=No;
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	old_interrupt_No=getvect(myNo);
	setvect(No,new_interrupt_No);
	asm sti;
#endif

	myEvent=0;
	IVTEntries[No]=this;
}

IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	setvect(myNo,old_interrupt_No);
	asm sti;
#endif
	IVTEntries[myNo]=0;

	old_interrupt_No=0;
	myNo=0;
	myEvent=0;
}

void IVTEntry::signal()
{
	if (myEvent!=0)
	{
		if (PCB::in_kernel_mode==1)
		{
			myWaitingEvents->insertSignal(myEvent);
			return;
		}
		IDStruct *help=new IDStruct();
		help->callerID=myEvent->myID;
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
}
