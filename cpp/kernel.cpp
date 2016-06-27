/*
 * kernel.cpp
 *
 *  Created on: Aug 23, 2015
 *      Author: Vladimir
 */

#include "kernel.h"
#include "KerSem.h"
#include "KernelEv.h"
#include "IDStruct.h"
#include "SleepQ.h"
#include "IVTEntry.h"
#include "EventQ.h"
#include "Pcb.h"
#include "Schedule.h"
#include <dos.h>

ptrInterrupt kernel::old_interrupt_routine60=0;
ptrInterrupt kernel::old_interrupt_routine61=0;
ptrInterrupt kernel::old_interrupt_timer08=0;

unsigned tempCx,tempBx;
unsigned ss_,sp_,ds_;
int kernel::timeCounter=0;

extern void tick();

void interrupt kernel::timer(...)
{
	(*old_interrupt_timer08)();

	tick();

	timeCounter++;
	SleepQ::timeSliceDecrease();


	if (PCB::in_kernel_mode==0)
	{
		SleepQ::wakeUpPCB();

		if (PCB::running->unlimited_time==1) { timeCounter=0; return; }

		if (timeCounter>=PCB::running->time)
		{
	#ifndef BCC_BLOCK_IGNORE
			asm{
				mov ss_,ss
				mov sp_,sp
			}
	#endif

			PCB::running->ss=ss_;
			PCB::running->sp=sp_;

			if (PCB::running!=PCB::idle_pcb){
				Scheduler::put(PCB::running);
			}
			PCB::running=Scheduler::get();
			if (PCB::running==0)
				PCB::running=PCB::idle_pcb;

			ss_=PCB::running->ss;
			sp_=PCB::running->sp;

	#ifndef BCC_BLOCK_IGNORE
			asm{
				mov ss,ss_
				mov sp,sp_
				mov bp,sp_
			}
	#endif
			timeCounter=0;
		}
	}
	else timeCounter=0;

}

void  interrupt kernel::enterKernelStack(...)
{
	PCB::in_kernel_mode=1;
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov ss_,ss
		mov sp_,sp
		mov ds_,ds
		mov cx,bp[cxOff]
		mov bx,bp[bxOff]
		mov tempCx,cx
		mov tempBx,bx
	}
#endif
	PCB::running->ss=ss_;
	PCB::running->sp=sp_;

	ss_=PCB::kernel->ss;
	sp_=PCB::kernel->sp;

#ifndef BCC_BLOCK_IGNORE
	asm{
		mov ss,ss_
		mov sp,sp_
		mov bp,sp_
		mov bx,tempBx
		mov cx,tempCx
		mov bp[bxOff],bx
		mov bp[cxOff],cx
	}
#endif

}

void interrupt kernel::exitKernelStack(...)
{
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov ss_,ss
		mov sp_,sp
	}
#endif

	PCB::kernel->ss=ss_;
	PCB::kernel->sp=sp_;

	if (PCB::running==0)
		PCB::running=PCB::idle_pcb;

	ss_=PCB::running->ss;
	sp_=PCB::running->sp;

#ifndef BCC_BLOCK_IGNORE
	asm{
		mov ss,ss_
		mov sp,sp_
		mov bp,sp_
	}
#endif
	SleepQ::wakeUpPCB();
	PCB::in_kernel_mode=0;
}


void kernel::systemSetup()
{
	setZeroes();
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	old_interrupt_routine60=getvect(ENTER_KERNEL);
	old_interrupt_routine61=getvect(EXIT_KERNEL);
	old_interrupt_timer08=getvect(TIMER_INT);

	setvect(ENTER_KERNEL,enterKernelStack);
	setvect(EXIT_KERNEL,exitKernelStack);
	setvect(TIMER_INT,timer);
	asm sti;

#endif
}

void kernel::systemReset()
{
	freeMemory();
#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
		in      al, 61h         //; Send acknowledgment without
		or      al, 10000000b   //;   modifying the other bits.
		out     61h, al         //;
		and     al, 01111111b   //;
		out     61h, al         //;
		mov     al, 20h         //; Send End-of-Interrupt signal
		out     20h, al         //;
		sti
	}
	asm cli;
	setvect(ENTER_KERNEL,old_interrupt_routine60);
	setvect(EXIT_KERNEL,old_interrupt_routine61);
	setvect(TIMER_INT,old_interrupt_timer08);
	asm sti;
#endif

}

void kernel::setZeroes()
{
	int i;

	for (i=0;i<MAX_PCB_NUM;i++)
		PCB::PCBS[i]=0;

	for (i=0;i<MAX_SEM_NUM;i++)
		KernelSem::SEMS[i]=0;

	for (i=0;i<MAX_EVE_NUM;i++)
			KernelEv::EVENTS[i]=0;
}


void kernel::freeMemory()
{
	int i;

	for (i=0;i<MAX_PCB_NUM;i++)
	{
		if (PCB::PCBS[i]!=0)
			delete PCB::PCBS[i];
	}

	for (i=0;i<MAX_SEM_NUM;i++)
	{
		if (KernelSem::SEMS[i]!=0)
			delete KernelSem::SEMS[i];
	}

	for (i=0;i<MAX_EVE_NUM;i++)
	{
		if (KernelEv::EVENTS[i]!=0)
			delete KernelEv::EVENTS[i];
	}

	delete PCB::PCBS;
	delete KernelEv::EVENTS;
	delete KernelSem::SEMS;
	delete IVTEntry::IVTEntries;
	delete IVTEntry::myWaitingEvents;
}

void kernel::systemService(){
	KernelEv *event=0;
	IDStruct *help=0;
	unsigned segment,offs;

	while(1){
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov segment,bx
		mov offs,cx
	}
	help=(IDStruct*)(((unsigned long)(segment) << 16) | (unsigned)(offs));//MK_FP(segment,offs);
#endif

	switch(help->functionCalled)
	{
		case PCB_START://1
		{
			PCB::PCBS[help->callerID]->start();
			break;
		}
		case PCB_WAIT_TO_COMPLETE://2
		{
			PCB::PCBS[help->callerID]->putInWaitingQueue();
			break;
		}
		case PCB_DELETE://3
		{
			delete PCB::PCBS[help->callerID];
			break;
		}
		case PCB_RELEASE_WAITING://4
		{
			PCB::PCBS[help->callerID]->releaseWaitingThreads();
			break;
		}
		case PCB_CREATE://5
		{
			help->callerID=PCB::pcb_create((Thread*)help->arg[0],*((StackSize*)help->arg[1]),*((Time*)help->arg[2]));
			break;
		}
		case PCB_DISPATCH: //6
		{

			PCB::dispatch();
			break;
		}
		case PCB_SLEEP: //7
		{
			PCB::sleep(*((Time*)help->arg[0]));
			break;
		}
		case SEM_WAIT: //8
		{
			help->callerID=KernelSem::SEMS[help->callerID]->wait(*((int*)help->arg[0]));
			break;
		}
		case SEM_SIGNAL: //9
		{
			KernelSem::SEMS[help->callerID]->signal();
			break;
		}
		case SEM_CREATE: //10
		{
			help->callerID=KernelSem::sem_create(*((int*)help->arg[0]));
			break;
		}
		case SEM_DELETE: //11
		{
			delete KernelSem::SEMS[help->callerID];
			break;
		}
		case SEM_VALUE: //12
		{
			(const int*)help->arg[0]=KernelSem::SEMS[help->callerID]->val();
			break;
		}
		case EVE_WAIT: //13
		{
			KernelEv::EVENTS[help->callerID]->wait();
			break;
		}
		case EVE_SIGNAL: //14
		{
			KernelEv::EVENTS[help->callerID]->signal();
			break;
		}
		case EVE_CREATE: //15
		{
			help->callerID=KernelEv::eve_create(*(IVTNo*)help->arg[0]);
			break;
		}
		case EVE_DELETE: //16
		{
			delete KernelEv::EVENTS[help->callerID];
			break;
		}
	}

	while ((event=IVTEntry::myWaitingEvents->getSignal())!=0) event->signal();

#ifndef BCC_BLOCK_IGNORE
	asm int EXIT_KERNEL;
#endif
	}
}


void kernel::idle()
{
	while(1){
	}
}
