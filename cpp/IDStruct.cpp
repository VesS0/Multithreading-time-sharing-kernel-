/*
 * IDStruct.cpp
 *
 *  Created on: Aug 22, 2015
 *      Author: Vladimir
 */

#include "IDStruct.h"

IDStruct::IDStruct() {
	ID callerPCB=0;
	int functionCalled=0;
	// TODO Auto-generated constructor stub

}

IDStruct::~IDStruct() {
	for (int i=0;i<2;i++) arg[i]=0;

	callerID=functionCalled=0;
}
