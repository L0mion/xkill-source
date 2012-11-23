#pragma once

#include <xkill-utilities/EventManager.h>
#include "AttributeController.h"

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