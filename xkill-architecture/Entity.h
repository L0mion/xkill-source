#pragma once

#include <xkill-utilities/EventManager.h>
#include "AttributeController.h"

/// Provides a unique id and keeps track of \ref ATTRIBUTES associated with that id.
/** 
Created by EntityFactory and stored in a EntityManager.

An Entity represents a bare bone game object and can be
almost anything based on its \ref ATTRIBUTES and 
the \ref COMPONENTS working on each attribute.

\ingroup ARCHITECTURE
*/

class Entity
{
private:
	int id;
	std::vector<AttributeController>* attributes;

public:
	Entity(int id)
	{
		this->id = id;
		attributes = new std::vector<AttributeController>;
	}

	void clean()
	{
		delete attributes;
	}

	~Entity()
	{
	}

	void deleteAttributes()
	{
		for(unsigned int i = 0; i < attributes->size(); i++)
			attributes->at(i).remove();
		attributes->clear();
	}

	void addAttribute(AttributeController attribute)
	{
		attributes->push_back(attribute);
	}

	
	/**
	Returns true if Entity contains an Attribute
	matching AttributeType. 
	*/
	bool hasAttribute(AttributeType type)
	{
		for(unsigned i=0; i<attributes->size(); i++)
		{
			if(type == attributes->at(i).type)
			{
				return true;
			}
		}
		return false;
	}

	/**
	Returns a std:vector<int> of all the matching index 
	of Attributes matching AttributeType. If Entity
	contains no matching Attributes, vector size is 0.
	*/
	std::vector<int> getAttributes(AttributeType type)
	{
		std::vector<int> matchingAttributes;
		for(unsigned i=0; i<attributes->size(); i++)
		{
			if(type == attributes->at(i).type)
			{
				matchingAttributes.push_back( attributes->at(i).index);
			}
		}
		return matchingAttributes;
	}

	int getID()
	{
		return id;
	}
};