#pragma once

#include <vector>
#include <queue>

#include "AttributePointer.h"
#include "Iterator.h"

#include "Entity.h"
#include "IAttributeStorage.h"
#include "AttributeController.h"


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
									//!< Owner 0 means no owner, and means the attribute.
									//!< Is deleted and ready to be reused.
	std::queue<int> deleted;		//!< Queue to keep track of deleted Indexe e.g. Indexes with "owner 0".
	int index_lastCreated;			//!< Index of the last created attribute
	AttributeType type;				//!< The AttributeType contained in AttributeStorage
	int num_attributes;

	AttributeController getAttributeController()
	{
		return AttributeController(this, index_lastCreated);
	}

public:

	std::vector<T> attributes;		//!< Each attribute.
	std::vector<int> owners;		//!< Owner of each attribute.

	AttributeStorage()
	{
		reset();

		// Init type
		T attribute;
		type = ((IAttribute*)&attribute)->getType();
	}

	int size()
	{
		return num_attributes;
	}

	void reset()
	{
		for(unsigned i=0; i<attributes.size(); i++)
		{
			IAttribute* a = (IAttribute*)&attributes[i];
			a->clean();
		}

		num_attributes = 0;
		attributes.clear();
		owners.clear();
		deleted = std::queue<int>();
	}

	IAttribute* getAttribute(int index)
	{
		IAttribute* a = (IAttribute*)&attributes[index];
		return a;
	}

	AttributeIterator<T> getIterator()
	{
		return AttributeIterator<T>(&attributes, &owners, this);
	}
	
	AttributeStorage(AttributeType type)
	{
		this->type = type;
	}

	~AttributeStorage()
	{
		for(unsigned i=0; i<attributes.size(); i++)
		{
			IAttribute* a = (IAttribute*)&attributes[i];
			a->clean();
		}
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
			index_lastCreated = deleted.front();
			deleted.pop();
			attributes[index_lastCreated] = T();
			owners[index_lastCreated]=owner->getID();
			
		}
		// FALSE: Create new attribute
		else
		{
			// Create attribute
			owners.push_back(owner->getID());
			index_lastCreated = (int)attributes.size();
			attributes.push_back(T());
		}
		num_attributes++;

		// Save access controller in Entity so it can be deleted later
		owner->addAttribute(getAttributeController());
		
		// Inform about creation
		Event_AttributeUpdated e(index_lastCreated, type);
		e.isCreated = true;
		EventManager::getInstance()->sendEvent(&e);

		// Get attribute
		return &attributes[index_lastCreated];
	}

	void deleteAttribute(int index)
	{
		// Clean Attribute before  deletion
		IAttribute* a = (IAttribute*)&attributes[index];
		a->clean();
		
		// Delete Attribute
		owners[index] = 0;
		num_attributes--;

		// Inform about deletion
		Event_AttributeUpdated e(index, type);
		e.isDeleted = true;
		EventManager::getInstance()->sendEvent(&e);

		// Allow Attribute to be reused
		deleted.push(index);
	}

	AttributePointer getLatestAttributeAsAttributePointer()
	{
		AttributePointer a;
		a.init(&attributes, index_lastCreated);
		return a;
	}

	
};

