/*
 * IVTEntry.h
 *
 *  Created on: Aug 28, 2015
 *      Author: Vladimir
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "const.h"
#include <iostream.h>

class KernelEv;
class EventQ;

class IVTEntry {

public:
	static IVTEntry **IVTEntries;
	static EventQ *myWaitingEvents;

	void signal();
	void oldRoute();
	void setEvent(KernelEv*);

	IVTEntry(IVTNo No,ptrInterrupt new_interrupt_No);
	virtual ~IVTEntry();

private:
	IVTNo myNo;
	ptrInterrupt old_interrupt_No;
	KernelEv *myEvent;
};

#define PREPAREENTRY(num,call_old_routine)\
void interrupt IVT_interrupt_number_##num(...); \
IVTEntry entry_number_##num(num, IVT_interrupt_number_##num); \
void interrupt IVT_interrupt_number_##num(...) \
{\
	entry_number_##num.signal();\
	if (call_old_routine) entry_number_##num.oldRoute();\
}

#endif /* IVTENTRY_H_ */

//kao da u makrou ne poziva signal. . . posto ne ulazi u signal u KernelEv, tj u signal IVTEntry. . .

