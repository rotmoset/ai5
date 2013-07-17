#include "Scope.h"
#include "StackMachine.h"

VariantReference<NameVariant> Scope::createName(const UnicodeString& name)
{
	NameVariant* n = new NameVariant(nullptr);
	lookup[name] = n;
	return n;
}

VariantReference<NameVariant> Scope::createIndexForName(const UnicodeString& name,int index)
{
	if (lookup.find(name) == lookup.end())
	{
		lookup[name] = new NameVariant(nullptr);
	}

	VariantReference<NameVariant> nameObj = lookup[name];

	addNameToIndex(index,nameObj);

	return nameObj;
}

void Scope::cleanup()
{
	Variant::cleanup();

	// Make sure the indexTable is all null's.
	for(size_t i=0;i<usedIndexes.size();i++)
	{
		indexTable[usedIndexes[i]] = VariantReference<NameVariant>();
	}

	lookup.clear();
	usedIndexes.clear();
}

void Scope::insertName(const UnicodeString& name,int index,VariantReference<NameVariant> nameVariant)
{
	lookup[name] = nameVariant;

	addNameToIndex(index,nameVariant);

}

void Scope::addNameToIndex(size_t index,const VariantReference<NameVariant>& nameVariant)
{
	if((index) >= indexTable.size())
	{
		indexTable.resize(index+1);
	}
	indexTable[index] = nameVariant;

	usedIndexes.push_back(index);

}
