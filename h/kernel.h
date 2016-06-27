/*
 * kernel.h
 *
 *  Created on: Aug 23, 2015
 *      Author: Vladimir
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include "Const.h"

class kernel {
public:
	static ptrInterrupt old_interrupt_routine60;
	static ptrInterrupt old_interrupt_routine61;
	static ptrInterrupt old_interrupt_timer08;

	static void interrupt enterKernelStack(...);
	static void interrupt exitKernelStack(...);
	static void interrupt timer(...);
	static void idle();
	static void systemSetup();
	static void systemReset();
	static void systemService();
	static void freeMemory();
	static void setZeroes();
	static int timeCounter;
	kernel();
	virtual ~kernel();
};

#endif /* KERNEL_H_ */
