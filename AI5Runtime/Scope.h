#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "Variant.h"
#include "NameVariant.h"
#include "PooledObject.h"
#include "types.h"

class StackMachine;

// Keeping this class optimized is VERY important.
class Scope: public ContainerVariant
{
public:
	static const VARIANT_TYPE TYPE = SCOPE;
	Scope(): ContainerVariant(TYPE), indexTable(128,0),usedIndexes(),enclosingScope(nullptr)
	{
		usedIndexes.reserve(16);
	}

	__forceinline NameVariant* getNameFromString(const UnicodeString &name)
	{
		return lookup[name];
	}
	__forceinline NameVariant* getNameFromIndex(int index)
	{
		NameVariant* result = indexTable[index];
		if(result == nullptr && enclosingScope != nullptr)
			result = enclosingScope->getNameFromIndex(index);
		return result;
	}
	NameVariant* createName(StackMachine* machine,const UnicodeString &name);
	NameVariant* createIndexForName(StackMachine* machine,const UnicodeString &name,int index);

	void insertName(const UnicodeString& name,int index,NameVariant* nameVariant);

	void setEnclosingScope(Scope* scope)
	{
		if(scope != nullptr)
			scope->addRef();
		if(this->enclosingScope != nullptr)
			this->enclosingScope->release();

		this->enclosingScope = scope;
	}

	// Make the scope ready for reusing.
	void cleanup();

	static Scope* createFromFactory(VariantFactory* factory);
	virtual int getChildContainersCount();
	virtual ContainerVariant* getChildContainer(int index);

private:

	Scope* enclosingScope;

	// The string->name lookup.
	// All names in the scope NEEDS to be in this map.
	std::unordered_map<UnicodeString,NameVariant*,UnicodeStringHasher,UnicodeStringComparator> lookup;

	// Used to provide super fast lookup of names in this scope.
	// Not all names are necessarily in this table. 
	std::vector<NameVariant*> indexTable;

	// List of used index in this table.
	// Used to avoid clearing the entire indexTable whenever a scope object is reused.
	std::vector<int> usedIndexes;

	void addNameToIndex(int index,NameVariant* nameVariant);
};