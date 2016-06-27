/*
 * Main.cpp
 *
 *  Created on: Aug 22, 2015
 *      Author: Vladimir
 */

#include "pcb.h"
#include "kernel.h"
#include <iostream.h>
#include <dos.h>
#include <IVTEntry.h>

extern int userMain(int argc,char *argv[]);

class UserMain:public Thread
{
public:
	UserMain(int argcc,char **argvv,StackSize stackSize= defaultStackSize,
			Time timeSlice=defaultTimeSlice):Thread(stackSize,timeSlice)
	{
		error=0;
		argc=argcc;
		argv=argvv;
	}
	void run()
	{
		error=userMain(argc,argv);
	}
	~UserMain()
	{
		waitToComplete();
	}
	int getErrors()
	{
		return error;
	}
private:
	int error;
	int argc;
	char **argv;
};

int main(int argc,char *argv[])
{

	kernel::systemSetup();


	PCB::idle_pcb=new PCB(IDLE_STACK_INITIALIZATION_MODE,defaultStackSize);
	PCB::kernel=new PCB(KERNEL_STACK_INITIALIZATION_MODE,defaultStackSize);
	PCB::running=new PCB();

	UserMain* user=new UserMain(argc,argv);

	user->start();

	delete user;

	delete PCB::idle_pcb;
	delete PCB::kernel;
	delete PCB::running;

	kernel::systemReset();

	cout<<"Program has finished"<<endl;

	return user->getErrors();
}
