/*
 * Semaphor.h
 *
 *  Created on: Aug 25, 2015
 *      Author: Vladimir
 */

// File: semaphor.h
#ifndef _semaphor_h_
#define _semaphor_h_

#include "Const.h"

class Semaphore {

public:

	Semaphore (int init=1);
	virtual ~Semaphore ();
	virtual int wait (int blockNo=0);
	virtual void signal();
 	int val () const; // Returns the current value of the semaphore

private:
 	 ID myImpl;
};
#endif
