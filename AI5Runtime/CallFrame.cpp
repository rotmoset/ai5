#include "CallFrame.h"
#include "StackMachine.h"
#include "DefaultVariant.h"

CallFrame::CallFrame(): Block(CALL_BLOCK)
{
}

CallFrame::~CallFrame(void)
{
}

void CallFrame::setup(StackMachine* machine,int returnAddress,int calledNumberOfArguments,CallFrame* parentFrame)
{
	this->returnAddress = returnAddress;
	this->stackPosition = machine->getDataStack()->position()-(calledNumberOfArguments+1);
	this->calledNumberOfArguments = calledNumberOfArguments;
	this->parentFrame = parentFrame;
	this->arguments.clear();
}

void CallFrame::leave(StackMachine*machine)
{
	unwindStack(machine,stackPosition);

	scope.reset();
}

Scope* CallFrame::getScope()
{
	return &scope;
}

void CallFrame::recycleInstance()
{
	CallFrame::returnInstance(this);
}

int CallFrame::getReturnAddress()
{
	return returnAddress;
}

CallFrame* CallFrame::getParentFrame()
{
	return this->parentFrame;
}

void CallFrame::addArgument(const Argument& arg)
{
	this->arguments.push_back(arg);
}

void CallFrame::loadArguments(StackMachine* machine,int total,int required)
{
	if(calledNumberOfArguments > total)
		throw RuntimeError(L"Too many arguments in function call");
	if(calledNumberOfArguments < required)
		throw RuntimeError(L"Too few arguments in function call");

	for(size_t argIndex=arguments.size();argIndex --> 0 ;)
	{
		Argument arg = arguments[argIndex];

		Variant* varArg;

		// Set arguments with missing value with Default.
		// The real default value is then set by the function body.
		if(argIndex >= calledNumberOfArguments)
		{
			varArg = &DefaultVariant::Instance;
			varArg->addRef();
		}
		else
		{
			varArg = machine->getDataStack()->pop();
		}

		if(arg.isByref && varArg->getLastName() != nullptr)
		{
			machine->addNameToLocalScope(arg.identifier,varArg->getLastName());
		}
		else
		{
			machine->setLocal(arg.identifier,varArg);
		}

		varArg->release();
	}
}