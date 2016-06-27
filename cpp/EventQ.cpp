/*
 * EventQ.cpp
 *
 *  Created on: Aug 30, 2015
 *      Author: Vladimir
 */

#include "EventQ.h"
#include "KernelEv.h"
#include <iostream.h>
#include <dos.h>
EventQ::EventNode::EventNode(KernelEv *ev){
	event=ev; next=0;
}


EventQ::EventQ() {
	first=last=0;
}

EventQ::~EventQ() {
	while (first!=0)
	{
		EventNode *old=first;
		first=first->next;
		delete old;
	}
	first=last=0;
}

KernelEv* EventQ::getSignal()
{
	KernelEv * ret=0;
	if (first!=0)
	{
		ret=first->event;
		EventNode *old=first;
		first=first->next;
		delete old;
		if (first==0) last=0;
	}
	return ret;
}

void EventQ::insertSignal(KernelEv *event)
{
	if (event==0) return;
	if (first==0)
		first=last=new EventNode(event);
	else
		last=last->next=new EventNode(event);
}

