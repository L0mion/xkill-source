#pragma once

#include "AttributeStorage.h"

/// A handle used by Entity to modify one of its Attributes
/** 
The most common usage is to delete all associated Attributes
upon deletion of an Entity.

\ingroup achitecture
*/

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