/*
 * WaitingQ.h
 *
 *  Created on: Aug 21, 2015
 *      Author: Vladimir
 */

#ifndef _WAITINGQ_H
#define _WAITINGQ_H

class PCB;

class WaitingQ
{
public:
	void blockRunningPCB();
	void unblockOne();
	void releaseWaitingThreads();
	~WaitingQ(){releaseWaitingThreads(); first=last=0;}
	WaitingQ(){first=0; last=0;}

private:
	class PCBNode{
		public:
		PCBNode();
		PCB *pcb;
		PCBNode *next;

		~PCBNode();

	};
	PCBNode *first,*last;

};


#endif
