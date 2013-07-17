#include "ListVariant.h"
#include "RuntimeError.h"
#include "VariantReference.h"

ListVariant::ListVariant(void): Variant(TYPE)
{
	list = shared_var_list(new std::vector<VariantReference<>>());
}


ListVariant::~ListVariant(void)
{
	cleanup();
}

void ListVariant::cleanup()
{
	Variant::cleanup();

	this->list = nullptr;

}

std::wostream& ListVariant::format(std::wostream& stream) const
{
	stream << L"ListVariant: " << toString()->getTerminatedBuffer();
	return stream;
}

double ListVariant::toFloating() const
{
	return 0.0;
}
__int64 ListVariant::toInteger64() const
{
	return 0;
}
int ListVariant::toInteger32() const
{
	return 0;
}
bool ListVariant::toBoolean() const
{
	return list->size() > 0;
}
shared_string ListVariant::toString() const
{
	UnicodeString* str = new UnicodeString();

	(*str)+=L"[";
	bool first = true;
	for(VariantReference<> var: *list)
	{
		if(!first)
			(*str)+=L", ";
		(*str) += *var.toString();

		first = false;
	}

	(*str) += L"]";

	return shared_string(str);
}


void ListVariant::addElement(const VariantReference<>& var)
{
	list->push_back(var);
}

const VariantReference<>&ListVariant::getElement(size_t index) const
{
	if(index >= list->size() || index < 0)
		throw RuntimeError(L"List index out of bounds!");
	return (*list)[index];
}

void ListVariant::setElement(size_t index,const VariantReference<>& var)
{
	if(index >= list->size() || index < 0)
		throw RuntimeError(L"List index out of bounds!");

	// Assign new value.
	(*list)[index] = var;
}


size_t ListVariant::size() const
{
	return list->size();
}

void ListVariant::deleteAt(size_t index)
{
	list->erase(list->begin()+index);
}

bool ListVariant::equal(Variant* other)
{
	if(getType() != other->getType())
		return false;

	ListVariant* otherList = static_cast<ListVariant*>(other);

	if(list->size() != otherList->list->size())
		return false;

	for(size_t index = 0; index < list->size(); index++)
	{
		if(!(*list)[index].equal((*otherList->list)[index]))
			return false;
	}

	return true;

}


IteratorVariant* ListVariant::iterate()
{
	return new ForwardIterator(this);
}


ListVariant::ForwardIterator::ForwardIterator(ListVariant* list):list(list),pos(0)
{
	list->addRef();
}
void ListVariant::ForwardIterator::cleanup()
{
	Variant::cleanup();
	list->release();
}
bool ListVariant::ForwardIterator::hasMore()
{
	return (size_t)pos < list->size();
}
const VariantReference<>& ListVariant::ForwardIterator::next()
{
	return list->getElement(pos++);
}