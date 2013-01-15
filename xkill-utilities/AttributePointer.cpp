#include "AttributePointer.h"


AttributePointer::AttributePointer()
{
	host = nullptr;
	index = -1;
}

void AttributePointer::init(void* host, int index)
{
	this->host = host;
	this->index = index;
}