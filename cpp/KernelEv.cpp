/*
 * KernelEv.cpp
 *
 *  Created on: Aug 28, 2015
 *      Author: Vladimir
 */

#include "KernelEv.h"
#include "PCB.h"
#include "schedule.h"
#include "IVTEntry.h"

KernelEv *ker[MAX_EVE_NUM];
KernelEv ** KernelEv::EVENTS=ker;
int KernelEv::indexCounter=0;

ID KernelEv::eve_create(IVTNo ivtNo)
{
	KernelEv *myEve=new KernelEv(ivtNo);
	return myEve->myID;
}

KernelEv::KernelEv(IVTNo ivtNo){
	myNo=ivtNo;
	myValue=1;
	myOwner=PCB::running;
	myID=indexCounter++;
	IVTEntry::IVTEntries[ivtNo]->setEvent(this);
	EVENTS[myID]=this;
	PCB::dispatch();
}

KernelEv::~KernelEv() {
	if (myOwner->state==BLOCKED)
	{
		myOwner->state=READY;
		Scheduler::put(myOwner);
	}
	IVTEntry::IVTEntries[myNo]->setEvent(0);
	if (myID>=0) EVENTS[myID]=0;

	myValue=0;
	myID=0;
	myNo=0;
	myOwner=0;
}

void KernelEv::wait()
{

	if (PCB::running!=myOwner) return;

	myOwner->state=BLOCKED;
	myValue=0;
	PCB::running=Scheduler::get();
}
void KernelEv::signal()
{
	if(myValue==0)
	{
		myOwner->state=READY;
		myValue=1;
		Scheduler::put(myOwner);
	}
	if (PCB::running!=0 && PCB::running!=myOwner) Scheduler::put(PCB::running);
	PCB::running=Scheduler::get();
}
