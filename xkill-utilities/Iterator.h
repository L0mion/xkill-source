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
    virtual int storageIndex() = 0;
};

// An Iterator to facilitate Attribute iteration
template <class T>
class AttributeIterator : public IAttributeIterator
{
private:
    std::vector<T>* _attributes;			//!< Each attribute.
    std::vector<int>* _allOwnerId;
	std::vector<Entity>* _allOwner;
    int _nextIndex;
	int _orderIndex;						//!< The order the Attribute occured, not counting empty attributes
	AttributeStorage<T>* _attributeStorage;
public:
	AttributeIterator()
	{
	}

    AttributeIterator(std::vector<T>* attributes, std::vector<int>* owners, AttributeStorage<T>* attributeStorage)
    {
		_attributes = attributes;
		_allOwnerId = owners;
		_attributeStorage = attributeStorage;
        resetIndex();

		// connect with owners
		Event_GetEntities e;										
		EventManager::getInstance()->sendEvent(&e);					
		_allOwner = e.entities;								
    }


    // Returns true if getNext will return a valid Item
    bool hasNext()
    {
        // Step to next Item or until end is reached
        while(_nextIndex < (int)_allOwnerId->size() && _allOwnerId->at(_nextIndex) == 0)
        {
            _nextIndex++;
        }

        // Returns TRUE if next Item is valid, otherwise end has been reached
		if(_nextIndex < (int)_allOwnerId->size())
			return true;
		else
		{
			// reset index
			resetIndex();
			return false;
		}
       
    }

    // Returns an Item and steps to next item
    AttributePtr<T> getNext()
    {
        // Fetch current item, and step to next item
        _nextIndex++;
		_orderIndex++;

		AttributePtr<T> pointer;
		pointer.init(_attributes, storageIndex());

        return pointer;
    }

	int storageIndex(T* attribute)
	{ 
		int index = attribute - &_attributes->at(0);
		return index;
	}

	// Returns the Index of the latest
	// Item aquired throught getNext()
	int storageIndex()
	{
		return _nextIndex-1;
	}


	void resetAllAttributes()
	{
		_attributeStorage->reset();
	}

	// Returns the order in which the current Attribute occurred
	// not counting empty attributes
	int orderIndex()
	{
		return _orderIndex;
	}

	// Returns how many attributes (including deleted ones) there is 
	int storageSize()
	{
		return (int)_allOwnerId->size();
	}

	

	// Returns nr of non-deleted attributes. Non optimized! 
	int size()
	{
		return _attributeStorage->size();
	}

	// Returns if the attribute at
	// index is deleted or not
	bool isDeleted(int index)
	{
		return _allOwnerId->at(index() == 0);
	}

	// Returns the Owner ID of the latest
	// Item aquired throught getNext()
	int ownerId()
	{
		return _allOwnerId->at(storageIndex());
	}

	// Returns the id of the owner
	// of the Attribute at index
	int ownerIdAt(int index)
	{
		return _allOwnerId->at(index);
	}

	// Returns the Owner of the latest
	// Item aquired throught getNext()
	Entity* owner()
	{
		return &_allOwner->at(ownerId());
	}

	// Returns the Owner of the latest
	// Item aquired throught getNext()
	Entity* ownerAt(int index)
	{
		return &_allOwner->at(ownerIdAt(index));
	}


    // Resets the Iterator to the beginning
    void resetIndex()
    {
        _nextIndex = 0;
		_orderIndex = -1;
    }

    // Returns an item in the vector regardless
    // if it is valid or not
	AttributePtr<T> at(int index)
    {
		AttributePtr<T> pointer;
		pointer.init(_attributes, index);

        return pointer;
    }

	// Returns an item in the vector regardless
    // if it is valid or not
    std::vector<T*> getMultiple(std::vector<int> indexVector)
    {
		std::vector<T*> attributeVector;
		for(int i=0; i<(int)indexVector.size(); i++)
			attributeVector.push_back(at(indexVector.at(i)));

        return attributeVector;
    }

	AttributePtr<T> createAttribute(Entity* e)
	{ 
		return _attributeStorage->createAttribute(e);
	}

	std::vector<int> getAllOwnerId()
	{
		return std::vector<int>(*_allOwnerId);
	}
};