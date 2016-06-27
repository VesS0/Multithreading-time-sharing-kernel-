/*
 * SleepQ.h
 *
 *  Created on: Aug 21, 2015
 *      Author: Vladimir
 */

#ifndef _SLEEPQ_H
#define _SLEEPQ_H

#include "Const.h"

class PCB;

class SleepQ{

public:
	static void sleep(Time timeToSleep);
	static void timeSliceDecrease();
	static void wakeUpPCB();

private:
		struct SleepNode{
		SleepNode(Time timeToSleep);
		~SleepNode();
		PCB *pcb;
		Time time;
		SleepNode *next;
	};

	static SleepNode *first; //
};



#endif

