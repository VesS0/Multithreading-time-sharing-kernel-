/*
 * SleepQ.cpp
 *
 *  Created on: Aug 21, 2015
 *      Author: Vladimir
 */

#include "SleepQ.h"
#include "PCB.h"
#include "Schedule.h"
#include <iostream.h>
SleepQ::SleepNode *SleepQ::first=0; //



void SleepQ::sleep(Time timeToSleep)
{
	SleepNode *newNode=new SleepNode(timeToSleep);
	if (first==0) first=newNode;
	else
	{
		if (newNode->time<first->time)
		{
			newNode->next=first;
			first->time-=newNode->time;
			first=newNode;
			return;
		}


		SleepNode *last=0,*curr=first;
		while (curr!=0 && newNode->time>=curr->time)
		{
			last=curr;
			newNode->time-=curr->time;
			curr=curr->next;
		}
		last->next=newNode;
		newNode->next=curr;
		if (curr!=0)curr->time-=newNode->time;
	}
}

void SleepQ::timeSliceDecrease()
{
		if (first!=0)
			first->time--;
}

void SleepQ::wakeUpPCB()
{
	while(first!=0 && first->time<=0)
	{
		SleepNode *old=first;
		first=first->next;
		delete old;
	}
}

SleepQ::SleepNode::~SleepNode()
	{
		pcb->state=READY;
		Scheduler::put(pcb);
		pcb=0;
		time=0;
		next=0;
	}

SleepQ::SleepNode::SleepNode(Time timeToSleep)
	{
		pcb=PCB::running;
		PCB::running->state=BLOCKED;
		time=timeToSleep;
		next=0;
	}
