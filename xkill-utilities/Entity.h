#pragma once

#include "dllUtilities.h"
#include "EventManager.h"
#include <vector>
enum AttributeType;
class AttributeController;


/// Provides a unique id and keeps track of \ref ATTRIBUTES associated with that id.
/** 
Created by EntityFactory and stored in a EntityManager.

An Entity represents a bare bone game object and can be
almost anything based on its \ref ATTRIBUTES and 
the \ref COMPONENTS working on each attribute.

\ingroup ARCHITECTURE
*/

class DLL_U Entity
{
private:
	int id;
	std::vector<AttributeController>* attributes;

public:
	Entity(int id);

	void clean();

	~Entity();

	void deleteAttributes();

	std::vector<AttributeController>*  getAttributeControllers()
	{
		return attributes;
	}

	void addAttribute(AttributeController attribute);

	
	/**
	Returns true if Entity contains an Attribute
	matching AttributeType. 
	*/
	bool hasAttribute(AttributeType type);

	/**
	Returns a std:vector<int> of all the matching index 
	of Attributes matching AttributeType. If Entity
	contains no matching Attributes, vector size is 0.
	*/
	std::vector<int> getAttributes(AttributeType type);

	void removeAttribute( AttributeType type );

	/**
	TODO
	*/
	IAttribute* getAttributeInterface(int index);

	int getID();
};