#include "Entity.h"

#include "AttributeController.h"

Entity::Entity( int id )
{
	this->id = id;
	attributes = new std::vector<AttributeController>;
}

void Entity::clean()
{
	delete attributes;
}

Entity::~Entity()
{

}

void Entity::deleteAttributes()
{
	for(unsigned int i = 0; i < attributes->size(); i++)
		attributes->at(i).remove();
	attributes->clear();
}

void Entity::addAttribute( AttributeController attribute )
{
	attributes->push_back(attribute);
}

bool Entity::hasAttribute( AttributeType type )
{
	for(unsigned i=0; i<attributes->size(); i++)
	{
		if(type == attributes->at(i).getType())
		{
			return true;
		}
	}
	return false;
}

std::vector<int> Entity::getAttributes( AttributeType type )
{
	std::vector<int> matchingAttributes;
	for(unsigned i=0; i<attributes->size(); i++)
	{
		if(type == attributes->at(i).getType())
		{
			matchingAttributes.push_back( attributes->at(i).getIndex());
		}
	}
	return matchingAttributes;
}

int Entity::getID()
{
	return id;
}


IAttribute* Entity::getAttributeInterface(int index)
{
	IAttribute* attribute = NULL;
	if(index<(int)attributes->size())
		attribute = attributes->at(index).getAttribute();
	return attribute;
}

void Entity::removeAttribute( AttributeType type )
{
	for(unsigned i=0; i<attributes->size(); i++)
	{
		if(type == attributes->at(i).getType())
		{
			attributes->at(i).remove();
			attributes->erase(attributes->begin()+i);
		}
	}
}
