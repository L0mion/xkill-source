#pragma once

#include "Math.h"

#include <vector>
#include <string>


struct DLL_U Floatx
{
	float x;
    float y;
};

struct DataItem
{
	union DataValue
    {
		bool		*_bool;
		int			*_int;
		float		*_float;
		Float3		*_float3;
		Float4		*_float4;
    };

	enum DataType
	{
		_INVALID,

		_INT,
		_BOOL,
		_FLOAT,
		_FLOAT3,
		_FLOAT4,

		_LAST
	};

	DataItem()
	{
		// inits all pointers in unon
		// to zero any member will do
		value._bool = NULL;

		label = "INVALID";
	}

	void clean()
	{
		if(type == _BOOL)
			delete value._bool;
		if(type == _INT)
			delete value._int;
		if(type == _FLOAT)
			delete value._float;
		if(type == _FLOAT3)
			delete value._float3;
		if(type == _FLOAT4)
			delete value._float4;
	}

	DataValue value;
	DataType type;
	std::string label;
};

class DataItemList
{
private:
	std::vector<DataItem> content;
	int index;

public:
	DataItemList()
	{
		reset();
	}

	~DataItemList()
	{
		// delete pointers inside DataItems
		for(int i=0; i<(int)content.size(); i++)
			content.at(i).clean();
	}

	void reset()
	{
		index = 0;
	}

	void addNotSupported()
	{
		DataItem d;
		d.label = "UNKOWN";
		content.push_back(d);
	}

	void add(bool x, std::string label)
	{
		DataItem d;
		d.label = label;
		d.value._bool = new bool(x);
		d.type = DataItem::_BOOL;
		content.push_back(d);
	}
	void add(int x, std::string label)
	{
		DataItem d;
		d.label = label;
		d.value._int =  new int(x);
		d.type = DataItem::_INT;
		content.push_back(d);
	}
	void add(float x, std::string label)
	{
		DataItem d;
		d.label = label;
		d.value._float = new float(x);
		d.type = DataItem::_FLOAT;
		content.push_back(d);
	}

	void add(Float3 x, std::string label)
	{
		DataItem d;
		d.label = label;
		d.value._float3 = new Float3(x);
		d.type = DataItem::_FLOAT3;
		content.push_back(d);
	}

	void add(Float4 x, std::string label)
	{
		DataItem d;
		d.label = label;
		d.value._float4 = new Float4(x);
		d.type = DataItem::_FLOAT4;
		content.push_back(d);
	}

	bool hasNext()
	{
		if( index < (int)content.size())
			return true;
		else
			return false;
	}

	DataItem getNext()
	{
		// make sure we're not accessing an empty vector
		if(!hasNext())
		{
			// the data does not exist, check your code
			// and make sure you do not access more or
			// less data than you have created
			throw 0;
		}

		// access data normaly
		DataItem nextItem = content.at(index);
		index++;
		return nextItem;
	}
};
