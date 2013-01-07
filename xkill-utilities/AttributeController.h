#pragma once

#include "IAttributeStorage.h"
#include "EventManager.h"

/// A handle used by Entity to modify one of its Attributes
/** 
The most common usage is to delete all associated Attributes
upon deletion of an Entity.

\ingroup ARCHITECTURE
*/

class AttributeController
{
private:
	IAttributeStorage* host;
public:
	int index;
	AttributeType type;

	AttributeController(IAttributeStorage* host, int index, AttributeType type)
	{
		this->host = host;
		this->index = index;
		this->type = type;
	}

	void remove()
	{
		host->deleteAttribute(index);
	}
};