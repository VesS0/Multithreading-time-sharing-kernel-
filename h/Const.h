/*
 * Const.h
 *
 *  Created on: Aug 22, 2015
 *      Author: Vladimir
 */

#ifndef CONST_H_
#define CONST_H_

#define NO_CALLER -4
#define MAIN_ID -3
#define KERNEL_ID -2
#define IDLE_ID -1

#define IDLE_STACK_INITIALIZATION_MODE 0
#define KERNEL_STACK_INITIALIZATION_MODE 1

#define PCB_START 1
#define PCB_WAIT_TO_COMPLETE 2
#define PCB_DELETE 3
#define PCB_RELEASE_WAITING 4
#define PCB_CREATE 5
#define PCB_DISPATCH 6
#define PCB_SLEEP 7
#define SEM_WAIT 8
#define SEM_SIGNAL 9
#define SEM_CREATE 10
#define SEM_DELETE 11
#define SEM_VALUE 12
#define EVE_WAIT 13
#define EVE_SIGNAL 14
#define EVE_CREATE 15
#define EVE_DELETE 16



#define ENTER_KERNEL 0x0060
#define EXIT_KERNEL 0x0061
#define TIMER_INT 0x0008


#define axOff 16
#define bxOff 14
#define cxOff 12
#define dxOff 10
#define esOff 8
#define dsOff 6
#define siOff 4
#define diOff 2
#define bpOff 0

enum STATE{NEW,READY,BLOCKED,FINISHED};

typedef int ID;
typedef void interrupt (*ptrInterrupt)(...);
typedef unsigned char IVTNo;

typedef unsigned int Time; // time, x 55ms
typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
const Time defaultTimeSlice = 2; // default = 2*55ms
#define MAX_PCB_NUM 1000
#define MAX_SEM_NUM 1000
#define MAX_EVE_NUM 1000
#define NUM_OF_ENTRIES 256




#endif /* CONST_H_ */
