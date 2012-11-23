#pragma once

//
// Pointer class
//

#include <vector>
#include <queue>
using namespace std;

#define ATTRIBUTE_CAST(a,b,c) &((std::vector<a>*)c->b.host)->at(c->b.index)

class IAttributeStorage
{
public:
	virtual void deleteAttribute(int index) = 0;
};

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


//
// Template to store manage Attributes
//

class Entity;
template <class T>
class AttributeStorage : public IAttributeStorage
{
private:
	vector<T> attributes;
	vector<int> owners;
	queue<int> deleted;
	int index;

	AttributeController getAttributeController()
	{
		return AttributeController(this, index);
	}

public:
	vector<T>* getAllAttributes()
	{
		return &attributes;
	}

	vector<int>* getAllOwners()
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

	AttributePointer getAttributePointer()
	{
		AttributePointer a;
		a.init(&attributes, index);
		return a;
	}
};
