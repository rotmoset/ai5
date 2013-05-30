#include "Integer64Variant.h"
#include <iostream>
#include "StackMachine.h"
#include <sstream>

Integer64Variant::Integer64Variant(const __int64 integer): Variant(INTEGER64), value(integer)
{
	//std::cout << "Creating new instance with value: " << integer << std::endl;
}



void Integer64Variant::print()
{
	std::wcout << "IntegerVariant: " << *toString() << std::endl;
}

__int64 Integer64Variant::toInteger()
{
	return value;
}

bool Integer64Variant::toBoolean()
{
	return value != 0;
}

double Integer64Variant::toFloating()
{
	return (double)value;
}

Integer64Variant* Integer64Variant::createFromFactory(VariantFactory* factory,__int64 value)
{
	return factory->create<Integer64Variant,__int64>(Variant::INTEGER64,value);
}

shared_string Integer64Variant::toString()
{
	std::wstringstream stream;
	stream << value;
	return create_shared_string(stream.str());
}