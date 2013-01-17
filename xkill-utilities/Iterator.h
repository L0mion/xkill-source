#pragma once

#include "AttributeStorage.h"
#include "EventManager.h"
#include "EntityStorage.h"

class Entity;

// Forward declaration of templates
template <class T> 
class AttributeStorage;

class DLL_U IAttributeIterator
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
    std::vector<int>* allOwnerId;
	std::vector<Entity>* allOwner;
    int nextIndex;
	AttributeStorage<T>* attributeStorage;
public:
	AttributeIterator()
	{
	}

    AttributeIterator(std::vector<T>* attributes, std::vector<int>* owners, AttributeStorage<T>* attributeStorage)
    {
        this->attributes = attributes;
        this->allOwnerId = owners;
		this->attributeStorage = attributeStorage;
        nextIndex = 0;

		// connect with owners
		Event_GetEntities e;										
		EventManager::getInstance()->sendEvent(&e);					
		allOwner = e.entities;								
    }

    // Returns true if getNext will return a valid Item
    bool hasNext()
    {
        // Step to next Item or until end is reached
        while(nextIndex < (int)allOwnerId->size() && allOwnerId->at(nextIndex) == 0)
        {
            nextIndex++;
        }

        // Returns TRUE if next Item is valid, otherwise end has been reached
		if(nextIndex < (int)allOwnerId->size())
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
	int storageSize()
	{
		return (int)allOwnerId->size();
	}

	// HACK: Returns nr of non-deleted attributes. Non optimized! 
	int size()
	{
		int tmp_nextIndex = nextIndex;
		
		int count = 0;
		resetIndex();
		while(hasNext())
		{
			getNext();
			count++;
		}

		nextIndex = nextIndex;
		return count;
	}

	// Returns if the attribute at
	// index is deleted or not
	bool isDeleted(int index)
	{
		return allOwnerId->at(index() == 0);
	}

	// Returns the Owner ID of the latest
	// Item aquired throught getNext()
	int ownerId()
	{
		return allOwnerId->at(index());
	}

	// Returns the id of the owner
	// of the Attribute at index
	int ownerIdAt(int index)
	{
		return allOwnerId->at(index);
	}

	// Returns the Owner of the latest
	// Item aquired throught getNext()
	Entity* owner()
	{
		return &allOwner->at(ownerId());
	}

	// Returns the Owner of the latest
	// Item aquired throught getNext()
	Entity* ownerAt(int index)
	{
		return &allOwner->at(ownerIdAt(index));
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
    // if it is valid or not
    std::vector<T*> getMultiple(std::vector indexVector)
    {
		std::vector<T> attributeVector;
		for(int i=0; i<indexVector->size(); i++)
			attributeVector.push_back(at(indexVector.at(i)));

        return attributeVector;
    }

    // Returns an item in the vector regardless
    // if it is valid or not, using the Index of
    // the supplied Iterator
    T* at(AttributePointer attrPointer)
    {
        return at(attrPointer.index);
    }

	int index(T* attribute)
	{ 
		int index = attribute - &attributes->at(0);
		return index;
	}

	AttributePointer attributePointer(T* attribute)
	{ 
		AttributePointer pointer;
		pointer.init(&attributes, index(attribute));
		return pointer;
	}

	T* createAttribute(Entity* e)
	{ 
		return attributeStorage->createAttribute(e);
	}
};