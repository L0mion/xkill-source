#pragma once

#include <vector>
#include <queue>

#include "IAttributeStorage.h"
#include "AttributeController.h"
#include "AttributePointer.h"
#include "AttributeType.h"
#ifdef DLL_EXPORT
#define PUSHBACK(a) ((a*)attributes)[numAttributes] = *((a*)attribute)
#define GROW(a) temp = new a
#define COPY(a) memcpy(attributes,temp,sizeof(a)
#define GROW_AND_COPY(a) GROW(RenderAttribute)[maxAttributes*2]; COPY(PositionAttribute)*numAttributes)
#endif
class attstor
{
private:
	unsigned int numAttributes,maxAttributes;
	unsigned int numDeleted,maxDeleted;
protected:
public:
	attstor(){}
	~attstor(){}
	IAttribute* attributes;
	unsigned int* owners;
	unsigned int* deleted;

	int index;
	AttributeType attributeType;
	IAttribute* getAttributes()
	{
		return attributes;
	}
	unsigned int* getOwners()
	{
		return owners;
	}
	unsigned int* getDeleted()
	{
		return deleted;
	}
	unsigned int size()
	{
		return numAttributes;
	}
	void grow()
	{
		IAttribute* temp;
		switch(attributeType)
		{
		case AttributeType::Position:
			GROW_AND_COPY(PositionAttribute);
			break;
		case AttributeType::Spatial:
			GROW_AND_COPY(SpatialAttribute);
			break;
		case AttributeType::Render:
			GROW_AND_COPY(RenderAttribute);
			break;
		}
	}
	void push_back(IAttribute* attribute,unsigned int owner)
	{
		switch(attributeType)
		{
		case AttributeType::Position:
			PUSHBACK(PositionAttribute);
			break;
		case AttributeType::Spatial:
			PUSHBACK(SpatialAttribute);
			break;
		case AttributeType::Render:
			PUSHBACK(RenderAttribute);
			break;
		}
		owners[numAttributes] = owner;
		if(numAttributes==maxAttributes)
		{
			grow();
		}
	}
	
	IAttribute* createAttribute(Entity* owner)
	{
		// TRUE: Reuse attribute
		if(numDeleted > 0)
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
};

#undef GROW_AND_COPY
#undef COPY
#undef GROW
#undef PUSHBACK






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
