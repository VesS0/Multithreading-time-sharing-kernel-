/*
 * EventQ.h
 *
 *  Created on: Aug 30, 2015
 *      Author: Vladimir
 */

#ifndef EVENTQ_H_
#define EVENTQ_H_

class KernelEv;

class EventQ {

public:
	EventQ();
	virtual ~EventQ();
	void insertSignal(KernelEv*);
	KernelEv* getSignal();

private:
	struct EventNode
	{
		KernelEv *event;
		EventNode *next;
		EventNode(KernelEv *ev);
		~EventNode(){event=0; next=0;}
	};
	EventNode *first,*last;
};

#endif /* EVENTQ_H_ */
