#include "AttributePointer.h"


AttributePointer::AttributePointer()
{
}

void AttributePointer::init(void* host, int index)
{
	this->host = host;
	this->index = index;
}