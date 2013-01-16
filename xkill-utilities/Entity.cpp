#include "Entity.h"

#include "AttributeController.h"
#include "EventManager.h"

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

std::vector<int>  Entity::getAttributesAsEnums()
{
	std::vector<int> enums;
	for(int i=0; i<(int)attributes->size(); i++)
	{
		enums.push_back(attributes->at(i).type);
	}
	return enums;
}

void Entity::addAttribute( AttributeController attribute )
{
	attributes->push_back(attribute);
}

bool Entity::hasAttribute( AttributeType type )
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

std::vector<int> Entity::getAttributes( AttributeType type )
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

int Entity::getID()
{
	return id;
}


DataItemList* Entity::getDataListFromAttribute(int index)
{
	DataItemList* list = NULL;
	if(index<(int)attributes->size())
		list = attributes->at(index).getDataList();
	return list;
}