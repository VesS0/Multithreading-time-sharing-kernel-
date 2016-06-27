/*
 * IDStruct.h
 *
 *  Created on: Aug 22, 2015
 *      Author: Vladimir
 */

#ifndef IDSTRUCT_H_
#define IDSTRUCT_H_

#include "Const.h"

class IDStruct {
public:
	ID callerID;
	int functionCalled;
	void* arg[3];

	IDStruct();
	virtual ~IDStruct();
};

#endif /* IDSTRUCT_H_ */
