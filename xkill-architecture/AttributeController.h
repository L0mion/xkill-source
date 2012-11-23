#pragma once

#include "AttributeStorage.h"

class AttributeController
{
private:
	IAttributeStorage* host;
	int index;
public:
	AttributeController(IAttributeStorage* host, int index)
	{
		this->host = host;
		this->index = index;
	}

	void remove()
	{
		host->deleteAttribute(index);
	}
};