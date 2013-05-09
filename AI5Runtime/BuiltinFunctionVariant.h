#pragma once
#include <string>
#include "FunctionVariant.h"
class StackMachine;

typedef void (*BuiltinFunctionPointer)(StackMachine*);

class BuiltinFunctionVariant :
	public FunctionVariant
{
public:
	BuiltinFunctionVariant(const std::string &name,BuiltinFunctionPointer func);
	~BuiltinFunctionVariant(void);
	void call(StackMachine* machine);
	void print();
private:
	BuiltinFunctionPointer func;
	std::string name;
};
