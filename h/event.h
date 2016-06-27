/*
 * event.h
 *
 *  Created on: Aug 28, 2015
 *      Author: Vladimir
 */

// File: event.h

#ifndef _event_h_
#define _event_h_

#include "Const.h"

class KernelEv;

class Event {
public:

	Event (IVTNo ivtNo);
	~Event ();
	void wait ();

protected:

	friend class KernelEv;
	void signal(); // can call KernelEv

private:

	ID myImpl;
};
#endif
