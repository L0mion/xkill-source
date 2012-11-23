#pragma once

//
// Pointer class
//

#include <vector>
#include <queue>
using namespace std;

template <class T>
class AttributePointer
{
private:
public:
	vector<T>* host;
	int index;

	AttributePointer()
	{
	}

	void init(vector<T>* host, int index)
	{
		this->host = host;
		this->index = index;
	}

	T* getAttribute()
	{
		return &host->at(index);
	}
};


// ==========
// Attributes
//

struct PositionAttribute
{
	float position;
};

struct SpatialAttribute
{
	AttributePointer<PositionAttribute> positionAttribute;

	float rotation;
	float scale;
};

struct RenderAttribute
{
	AttributePointer<SpatialAttribute> spatialAttribute;

	bool transparent;
	bool tessellation;
	int meshID;
	int textureID;
};


class IAttributes
{
public:
	virtual void deleteAttribute(int index) = 0;
};

class AttributeController
{
private:
	IAttributes* host;
	int index;
public:
	AttributeController(IAttributes* host, int index)
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
class AttributeStorage : public IAttributes
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

	AttributePointer<T> getAttributePointer()
	{
		AttributePointer<T> a;
		a.init(&attributes, index);
		return a;
	}
};

