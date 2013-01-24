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
	int index;
	AttributeType type;
public:
	AttributeController(IAttributeStorage* host, int index)
	{
		this->host = host;
		this->index = index;
		this->type = getAttribute()->getType();
	}

	IAttribute* getAttribute()
	{
		return host->getAttribute(index);
	}

	void remove()
	{
		host->deleteAttribute(index);
	}

	AttributeType getType()
	{
		return type;
	}
	int getIndex()
	{
		return index;
	}
};