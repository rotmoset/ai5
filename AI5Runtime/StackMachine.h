#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>
#include "Static.h"
#include "DataStack.h"
#include "Scope.h"
#include "CallFrame.h"
#include "BuiltinFunctionVariant.h"
#include "macro.h"
#include "RuntimeError.h"
#include "SimplePool.h"
#include "BlockStack.h"

class Instruction;

using std::shared_ptr;
using std::vector;

// This represents the virtual machine.
// It controls program counter, stacks, tables, scopes and memory allocation.
class StackMachine
{
private:
	static const int CALL_STACK_SIZE		= 1024;
	static const int DATA_STACK_SIZE		= 65536;
public:
	StackMachine(shared_ptr<vector<shared_ptr<StaticData>>> statics,
					shared_ptr<vector<shared_ptr<Instruction>>> program);
	~StackMachine(void);
	// These methods are called by instructions, so they need
	// to be inlined for maximum speed.
	__forceinline void advanceCounter();
	__forceinline void jumpRelative(int offset);
	__forceinline void jumpAbsolute(int position);
	__forceinline int getCurrentAddress();
	__forceinline StaticData* getStaticData(int index);
	__forceinline DataStack* getDataStack();
	__forceinline VariantFactory* getVariantFactory();
	__forceinline BlockStack* getBlockStack();
	inline void setCurrentCallFrame(CallFrame* frame);
	inline CallFrame* getCurrentCallFrame();
	static StackMachine* LoadFromStructuredData(const std::wstring& filename);
	int start();
	void terminate();
	__forceinline NameVariant* getNearestName(NameIdentifier identifier);
	__forceinline void setNearest(NameIdentifier identifier,Variant* variant,bool asConst=false);
	__forceinline void setLocal(NameIdentifier identifier,Variant* variant,bool asConst=false);
	__forceinline void setGlobal(NameIdentifier identifier,Variant* variant,bool asConst=false); 
	__forceinline void addNameToLocalScope(NameIdentifier identifier,NameVariant* name);
	void addBuiltInFunction(const std::wstring &name,BuiltinFunctionPointer function);
	void addMacro(const std::wstring &name,MACRO_FUNCTION macroFunc);
	MACRO_FUNCTION getMacro(int staticIndex);
private:
	shared_ptr<vector<shared_ptr<Instruction>>> program;
	shared_ptr<vector<shared_ptr<StaticData>>> staticsTable;
	BlockStack blockStack; 
	CallFrame* currentCallFrame;
	std::unordered_map<std::wstring,MACRO_FUNCTION> macros;
	Scope globalScope;
	DataStack dataStack;
	VariantFactory variantFactory;
	bool terminated;
	int programCounter;
	static const int RECURSION_LIMIT = 1024;
};

void StackMachine::jumpRelative(int offset)
{
	programCounter += offset;
}
void StackMachine::jumpAbsolute(int position)
{
	programCounter = position;
}
StaticData* StackMachine::getStaticData(int index)
{
	return (*staticsTable)[index].get();
}

DataStack* StackMachine::getDataStack()
{
	return &dataStack;
}

BlockStack* StackMachine::getBlockStack()
{
	return &blockStack;
}

void StackMachine::advanceCounter()
{
	programCounter++;
}

VariantFactory* StackMachine::getVariantFactory()
{
	return &variantFactory;
}

int StackMachine::getCurrentAddress()
{
	return programCounter;
}

void StackMachine::setCurrentCallFrame(CallFrame* frame)
{
	this->currentCallFrame = frame;
}

CallFrame* StackMachine::getCurrentCallFrame()
{
	return this->currentCallFrame;
}


NameVariant* StackMachine::getNearestName(NameIdentifier identifier)
{
	if(currentCallFrame != nullptr)
	{
		NameVariant* name = currentCallFrame->getScope()->getNameFromIndex(identifier.localId);
		if(name != nullptr)
			return name;
	}

	NameVariant* name = globalScope.getNameFromIndex(identifier.globalId);

	// If name not found from index, do a "hard" search with the name
	// Add it as an index afterwords so next lookup is FAST.
	if(name == nullptr)
	{
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
		name = globalScope.getNameFromString(*std::static_pointer_cast<StaticName>(staticData)->getName());
		// If name is still nullptr, throw error!
		globalScope.createIndexForName(this,*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.globalId);
	}

	return name;
}

// This function sets the value for a name in the nearest scope where it's found.
// If it isn't found it is added to the local scope, and if there is no local scope, to the global scope.
void StackMachine::setNearest(NameIdentifier identifier,Variant* variant,bool asConst)
{
	// Search for name in local and global scope.
	NameVariant* foundName = nullptr;
	if(currentCallFrame != nullptr)
		foundName = currentCallFrame->getScope()->getNameFromIndex(identifier.localId);
	if(foundName == nullptr)
		foundName = globalScope.getNameFromIndex(identifier.globalId);

	// If not found, add it as a new name to the nearest scope.
	if(foundName == nullptr)
	{
		Scope* targetScope = nullptr;
		if(currentCallFrame != nullptr)
			targetScope = currentCallFrame->getScope();
		else
			targetScope = &globalScope;
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];

		// The name may be defined without this index. This doesn't matter as the createIndexForName will check
		// if the name is already defined.
		foundName = targetScope->createIndexForName(this,*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.localId);
	}


	foundName->setValue(variant);

	if(asConst)
	{
		foundName->markAsConst();
	}
}


void StackMachine::setLocal(NameIdentifier identifier,Variant* variant,bool asConst)
{
	Scope* targetScope = &globalScope;
	if(currentCallFrame != nullptr)
		targetScope = currentCallFrame->getScope();

	NameVariant* name = targetScope->getNameFromIndex(identifier.localId);

	if(name == nullptr)
	{
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
		name = targetScope->createIndexForName(this,*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.localId);
	}

	name->setValue(variant);

	if(asConst)
	{
		name->markAsConst();
	}

}
void StackMachine::setGlobal(NameIdentifier identifier,Variant* variant,bool asConst)
{
	NameVariant* foundName = globalScope.getNameFromIndex(identifier.globalId);
	if(foundName == nullptr)
	{		
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
		foundName = globalScope.createIndexForName(this,*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.globalId);
	}
	foundName->setValue(variant);

	if(asConst)
	{
		foundName->markAsConst();
	}
}

void StackMachine::addNameToLocalScope(NameIdentifier identifier,NameVariant* name)
{
	Scope* targetScope = &globalScope;
	if(currentCallFrame != nullptr)
		targetScope = currentCallFrame->getScope();

	std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
	const std::wstring& strName = *std::static_pointer_cast<StaticName>(staticData)->getName();
	targetScope->insertName(strName,identifier.localId,name);
}
