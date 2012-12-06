#pragma once

#include <vector>
#include <queue>

#include "IAttributeStorage.h"
#include "AttributeController.h"
#include <xkill-utilities/AttributePointer.h>
#include <xkill-utilities/AttributeType.h>
#include "Entity.h"


/// Template class for storing and creating \ref ATTRIBUTES in a uniform manner.
/** 
IMPORTANT: The owner (Entity) of each attribute is stored in "owners".
If owner is 0 it means the attribute is deleted and ready to be reused
when a new Entity is created.

\ingroup ARCHITECTURE
*/

template <class T>
class AttributeStorage : public IAttributeStorage
{
private:
	std::vector<T> attributes;		//!< Each attribute.
	std::vector<int> owners;		//!< Owner of each attribute.
									//!< Owner 0 means no owner, and means the attribute.
									//!< Is deleted and ready to be reused.
	std::queue<int> deleted;		//!< Queue to keep track of deleted Indexe e.g. Indexes with "owner 0".
	int index;
	AttributeType type;				//!< The AttributeType contained in AttributeStorage

	AttributeController getAttributeController()
	{
		return AttributeController(this, index);
	}

public:
	AttributeStorage()
	{
	}

	void init(AttributeType type)
	{
		this->type = type;
	}

	std::vector<T>* getAllAttributes()
	{
		return &attributes;
	}

	std::vector<int>* getAllOwners()
	{
		return &owners;
	}

	/** 
	Creates space for an attribute and returns a
	pointer to that space.
	The caller is responsible for filling
	out the attribute.
	*/
	T* createAttribute(Entity* owner)
	{
		// TRUE: Reuse attribute
		if(deleted.size() > 0)
		{
			// Reuse attribute
			index = deleted.front();
			deleted.pop();
			owners[index]=owner->getID();
		}
		// FALSE: Create new attribute
		else
		{
			// Create attribute
			owners.push_back(owner->getID());
			index = (int)attributes.size();
			attributes.push_back(T());
		}

		// Save access controller in Entity so it can be deleted later
		owner->addAttribute(getAttributeController());

		// Get attribute
		return &attributes[index];
	}

	void deleteAttribute(int index)
	{
		owners[index] = 0;
		deleted.push(index);
	}

	AttributePointer getLatestAttributeAsAttributePointer()
	{
		AttributePointer a;
		a.init(&attributes, index);
		return a;
	}
};

