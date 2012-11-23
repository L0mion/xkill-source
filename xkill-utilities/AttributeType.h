#pragma once


#include "dllUtilities.h"
#define ATTRIBUTE_CAST(Type,Object,Parent) &((std::vector<Type>*)Parent->Object.host)->at(Parent->Object.index)


//! Attribute Pointer
/*!
Used to gain access to a specifc attribute in the attibute memory space
\ingroup xkill-utilities
*/
class DLL AttributePointer
{
public:
	void* host; //!< A void pointer to a std::vector with attributes of an specific type, manual casting of void
	unsigned int index;

	AttributePointer()
	{
	}

	void init(void* host, int index)
	{
		this->host = host;
		this->index = index;
	}
};


//! IAttribute
/*!
Simple/empty attribute supplying an interface for simpler
grouping of attributes.
\ingroup xkill-utilities
*/
struct DLL IAttribute
{
};


//! PositionAttribute
/*!
Storing a position attribute
\ingroup xkill-utilities
*/
struct DLL PositionAttribute : public IAttribute
{
	float position;
};

//! SpatialAttribute
/*!
Storing a spatial attribute
\ingroup xkill-utilities
*/
struct DLL SpatialAttribute : public IAttribute
{
	AttributePointer positionAttribute;

	float rotation;
	float scale;
};


//! RenderAttribute
/*!
Storing a render attribute
\ingroup xkill-utilities
*/
struct DLL RenderAttribute : public IAttribute
{
	AttributePointer spatialAttribute;

	bool transparent;
	bool tessellation;
	int meshID;
	int textureID;
};

//! PhysicsAttribute
/*!
Storing a physics attribute
\ingroup xkill-utilities
*/
struct DLL PhysicsAttribute : public IAttribute
{
	AttributePointer spatialAttribute;
	float velocity[4];
	float rotationVelocity[4];
	float mass;

	bool added;
	bool alive;
};
