#pragma once

#include "FastStack.h"
#include "NameStorage.h"

class StackMachine;

class CallFrame
{
public:
	CallFrame(int returnAddress);
	~CallFrame(void);

	int getReturnAddress()
	{
		return returnAddress;
	}

	void attachName(Name* name)
	{
		locals.push(name);

	}
	void detachNames()
	{
		while(locals.size() > 0)
		{
			locals.pop()->popLocal();
		}
	}

private:
	int returnAddress;
	FastStack<Name*> locals;
};
