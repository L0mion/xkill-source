#pragma once

#include <vector>
#include <queue>

#include "IAttributeStorage.h"
#include "AttributeController.h"
#include "AttributePointer.h"

class Entity;
template <class T>
class AttributeStorage : public IAttributeStorage
{
private:
	std::vector<T> attributes;
	std::vector<int> owners;
	std::queue<int> deleted;
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

	// Create space for attribute and return 
	// pointer to that space
	// The caller is then responsible to fill
	// out the attribute
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

	AttributePointer<T> getAttributePointer()
	{
		AttributePointer<T> a;
		a.init(&attributes, index);
		return a;
	}
};
