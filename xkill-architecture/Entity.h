#pragma once

#include "EventManager.h"
#include "AttributeType.h"

class Entity
{
private:
	int id;
	vector<AttributeController> attributes;

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
		for each(AttributeController c in attributes)
			c.remove();
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