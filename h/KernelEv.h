/*
 * KernelEv.h
 *
 *  Created on: Aug 28, 2015
 *      Author: Vladimir
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "const.h"

class PCB;

class KernelEv {
public:
	static KernelEv **EVENTS;
	static int indexCounter;
	static ID eve_create(IVTNo ivtNo);

	KernelEv(IVTNo ivtNo);
	virtual ~KernelEv();
	void wait();
	void signal();

private:
	int myValue;
	ID myID;
	IVTNo myNo;
	PCB* myOwner;

	friend class IVTEntry;
};

#endif /* KERNELEV_H_ */
