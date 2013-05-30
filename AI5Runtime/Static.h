#pragma once

#include <vector>
#include <memory>
#include "StringVariant.h"
#include "FloatingVariant.h"
#include "Integer64Variant.h"

typedef unsigned char STATIC_TYPE;


class StaticData
{
public:
	typedef std::shared_ptr<StaticData> PTR;
	const static STATIC_TYPE STRING = 0;
	const static STATIC_TYPE BINARY = 1;
	const static STATIC_TYPE NAME = 2;
	const static STATIC_TYPE FLOATING = 3;
	const static STATIC_TYPE INTEGER64 = 4;
	StaticData(STATIC_TYPE type): type(type)
	{
	}
	__forceinline STATIC_TYPE getType()
	{
		return type;
	}
private:
	STATIC_TYPE type;

};

class StaticString: public StaticData
{
public:
	StaticString(const std::wstring& str): StaticData(STRING), strVar(create_shared_string(str))
	{

	}
	StringVariant* getVariant()
	{
		return &strVar;
	}
private:
	StringVariant strVar;
};

class StaticName: public StaticData
{
public:
	StaticName(const std::wstring& str): StaticData(NAME),name(create_shared_string(str))
	{

	}

	__forceinline shared_string getName()
	{
		return name;
	}
private:
	shared_string name;
};

class StaticInteger64: public StaticData
{
public:
	StaticInteger64(__int64 value): StaticData(INTEGER64),variant(value)
	{

	}

	__forceinline Integer64Variant* getVariant()
	{
		return &variant;
	}
private:
	Integer64Variant variant;
};


class StaticFloating: public StaticData
{
public:
	StaticFloating(double value): StaticData(FLOATING),variant(value)
	{

	}

	__forceinline FloatingVariant* getVariant()
	{
		return &variant;
	}
private:
	FloatingVariant variant;
};


