/*
 * WaitingQ.cpp
 *
 *  Created on: Aug 21, 2015
 *      Author: Vladimir
 */

#include "WaitingQ.h"
#include "PCB.h"
#include "schedule.h"

void WaitingQ::blockRunningPCB(){
	if (first==0){
		first=new PCBNode();
		last=first;
	}
	else{
		last->next=new PCBNode();
		last=last->next;
	}

	PCB::running=Scheduler::get();
}

void WaitingQ::releaseWaitingThreads()
	{
		while (first!=0)
		{
			PCBNode *old=first;
			first=first->next;
			delete old;
		}
		first=last=0;
	}

void WaitingQ::unblockOne(){
	if (first!=0){
		PCBNode *old=first;
		first=first->next;
		delete old;
		if (first==0) last=first=0;
	}
}

WaitingQ::PCBNode::PCBNode()
	{
		pcb=PCB::running;
		next=0;
		pcb->state=BLOCKED;
	}

WaitingQ::PCBNode::~PCBNode()
{
	pcb->state=READY;
	Scheduler::put(pcb);
	pcb=0;
	next=0;
}
