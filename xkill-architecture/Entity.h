#pragma once

#include <xkill-utilities/EventManager.h>
#include "AttributeController.h"

/// Provies a unique id and keeps track of \ref attributes associeted with that id
/** 
Created by EntityFactory and stored in a EntityManager.

A Entity represents a barebone game object and can be
almost anything based in on its \ref attributes and 
the \ref components working on each attribute.
\ingroup achitecture
*/

class Entity
{
private:
	int id;
	std::vector<AttributeController> attributes;

public:
	Entity(int id)
	{
		this->id = id;
	}

	~Entity()
	{
	}

	void deleteAttributes()
	{
		for(unsigned int i = 0; i < attributes.size(); i++)
			attributes[i].remove();
	}

	void addAttribute(AttributeController attribute)
	{
		attributes.push_back(attribute);
	}

	int getID()
	{
		return id;
	}
};