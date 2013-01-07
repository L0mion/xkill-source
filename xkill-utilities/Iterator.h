#pragma once

#include "AttributeStorage.h"
class Entity;

// Forward declaration of templates
template <class T> 
class AttributeStorage;

class IAttributeIterator
{
public:
    virtual int index() = 0;
};

// An Iterator to facilitate Attribute iteration
template <class T>
class AttributeIterator : public IAttributeIterator
{
private:
    std::vector<T>* attributes;        //!< Each attribute.
    std::vector<int>* owners;
	AttributeStorage<T>* storage;

    int nextIndex;
	
public:
	AttributeIterator()
	{
	}
    AttributeIterator(std::vector<T>* attributes, std::vector<int>* owners, AttributeStorage<T>* storage)
    {
        this->attributes = attributes;
        this->owners = owners;
		this->storage = storage;

        nextIndex = 0;
    }

    // Returns true if getNext will return a valid Item
    bool hasNext()
    {
        // Step to next Item or until end is reached
        while(nextIndex < (int)owners->size() && owners->at(nextIndex) == 0)
        {
            nextIndex++;
        }

        // Returns TRUE if next Item is valid, otherwise end has been reached
		if(nextIndex < (int)owners->size())
			return true;
		else
		{
			// reset index
			resetIndex();
			return false;
		}
       
    }

    // Returns an Item and steps to next item
    T* getNext()
    {
        // Fetch current item, and step to next item
        nextIndex++;
        return &attributes->at(index());
    }

    // Returns the Index of the latest
    // Item aquired throught getNext()
    int index()
    {
        return nextIndex-1;
    }

	// Returns how many attributes (including deleted ones) there is 
	int size()
	{
		return (int)owners->size();
	}

	// Returns the Owner of the latest
	// Item aquired throught getNext()
	int ownerId()
	{
		return owners->at(index());
	}

	// Returns the id of the owner
	// of the Attribute at index
	int ownerIdAt(int index)
	{
		return owners->at(index);
	}

    // Resets the Iterator to the beginning
    void resetIndex()
    {
        nextIndex = 0;
    }

    // Returns an item in the vector regardless
    // if it is valid or not
    T* at(int index)
    {
        return &attributes->at(index);
    }

    // Returns an item in the vector regardless
    // if it is valid or not, using the Index of
    // the supplied Iterator
    T* at(AttributePointer attrPointer)
    {
        return at(attrPointer.index);
    }

	T* createAttribute(Entity* owner)
    {
        return createAttribute(owner);
    }
};