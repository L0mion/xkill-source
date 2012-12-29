#pragma once

#include "AttributeStorage.h"

// Forward declaration of templates
template <class T> 
class AttributeStorage;

class IAttributeIterator
{
public:
    virtual int getCurrentIndex() = 0;
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
        while(nextIndex < owners->size() && owners->at(nextIndex) == 0)
        {
            nextIndex++;
        }

        // Returns TRUE if next Item is valid, otherwise end has been reached
        return nextIndex < owners->size() ? true : false;
    }

    // Returns an Item and steps to next item
    T* getNext()
    {
        // Fetch current item, and step to next item
        nextIndex++;
        return &attributes->at(getCurrentIndex());
    }

    // Retuns the Index of the latest
    // Item aquired throught getNext()
    int getCurrentIndex()
    {
        return nextIndex-1;
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
    T* at(IAttributeIterator* iterator)
    {
        return at(iterator->getCurrentIndex);
    }

	T* createAttribute(Entity* owner)
    {
        return createAttribute(owner);
    }
};