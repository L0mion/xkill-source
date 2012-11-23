#pragma once

#include "dllUtilities.h"

class DLL AttributePointer
{
public:
	void* host;
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

struct DLL IAttribute
{
};

struct DLL AttributeLock
{
	IAttribute* attributes;
	// Mutex
	IAttribute* getAttributes()
	{
		//if(Mutex.aquire())
		return attributes;
	}
};

enum AttributeType
{
	Position = 0,
	Spatial,
	Render
};



struct DLL PositionAttribute : public IAttribute
{
	float position;
};

struct DLL SpatialAttribute : public IAttribute
{
	AttributePointer positionAttribute;

	float rotation;
	float scale;
};

struct DLL RenderAttribute : public IAttribute
{
	AttributePointer spatialAttribute;

	bool transparent;
	bool tessellation;
	int meshID;
	int textureID;
};

