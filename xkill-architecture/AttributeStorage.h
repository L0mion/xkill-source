#pragma once

#include <vector>
#include <queue>

#include "IAttributeStorage.h"
#include "AttributeController.h"
#include <xkill-utilities/AttributePointer.h>
class Entity;


/// Template class for storing and creating \ref attributes in a uniform manner.
/** 
IMPORTANT: The owner (Entity) of each attribute is stored in "owners".
if owner is 0 it means the attribute is deleted and ready to be reasused
when a new Entity is created. 
\ingroup achitecture
*/

template <class T>
class AttributeStorage : public IAttributeStorage
{
private:
	std::vector<T> attributes;		//!< Each attribute.
	std::vector<int> owners;		//!< Owner of each attribute.
									//!< owner 0 means no owner, and means the attribute
									//!< is deleted and ready to be reause
	std::queue<int> deleted;		//!< Queue to keep track of deleted Indexe e.g. Indexes with "owner 0".
	int index;

	AttributeController getAttributeController()
	{
		return AttributeController(this, index);
	}

public:

	std::vector<T>* getAllAttributes()
	{
		return &attributes;
	}

	std::vector<int>* getAllOwners()
	{
		return &owners;
	}

	/** 
	Creates space for attribute and return 
	pointer to that space.
	The caller is then responsible to fill
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

	AttributePointer getAttributePointer()
	{
		AttributePointer a;
		a.init(&attributes, index);
		return a;
	}
};

