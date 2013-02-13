#pragma once

#include "Math.h"
#include "AttributePointer.h"
#include "AttributeType.h"

#include <vector>
#include <string>

struct DataItem
{
	union DataValue
    {
		bool		*_bool;
		int			*_int;
		float		*_float;
		Float2		*_float2;
		Float3		*_float3;
		Float4		*_float4;
		Float4x4	*_float4x4;
		std::string	*_string;
    };

	enum DataType
	{
		_INVALID,

		_INT,
		_BOOL,
		_FLOAT,
		_FLOAT2,
		_FLOAT3,
		_FLOAT4,
		_FLOAT4X4,
		_STRING,
		_ATTRIBUTE_POINTER,

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
		if(type == _FLOAT2)
			delete value._float2;
		if(type == _FLOAT3)
			delete value._float3;
		if(type == _FLOAT4)
			delete value._float4;
		if(type == _FLOAT4X4)
			delete value._float4x4;
		if(type == _STRING)
			delete value._string;
		if(type == _ATTRIBUTE_POINTER)
			delete value._int;
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
	void add(unsigned x, std::string label)
	{
		DataItem d;
		d.label = label;
		d.value._int =  new int((int)x);
		d.type = DataItem::_INT;
		content.push_back(d);
	}
	void add(short x, std::string label)
	{
		DataItem d;
		d.label = label;
		d.value._int =  new int((int)x);
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

	void add(Float2 x, std::string label)
	{
		DataItem d;
		d.label = label;
		d.value._float2 = new Float2(x);
		d.type = DataItem::_FLOAT2;
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

	void add(Float4x4 x, std::string label)
	{
		DataItem d;
		d.label = label;
		d.value._float4x4 = new Float4x4(x);
		d.type = DataItem::_FLOAT4X4;
		content.push_back(d);
	}
	void add(std::string x, std::string label)
	{
		DataItem d;
		d.label = label;
		d.value._string = new std::string(x);
		d.type = DataItem::_STRING;
		content.push_back(d);
	}
	void add_Enum(int x, std::string label)
	{
		DataItem d;
		d.label = label;
		d.value._int =  new int(x);
		d.type = DataItem::_INT;
		content.push_back(d);
	}
	void add(IAttributePtr* x, std::string label)
	{
		DataItem d;
		d.label = label;
		d.value._int =  new int(x->index());
		d.type = DataItem::_ATTRIBUTE_POINTER;
		content.push_back(d);
	}

	void add_NotSupported(std::string label)
	{
		DataItem d;
		d.label = label;
		d.value._string = new std::string("NOT_SUPPORTED");
		d.type = DataItem::_STRING;
		content.push_back(d);
	}

	void get(bool* x)
	{
		*x = *getNext()->value._bool;
	}
	void get(int* x)
	{
		*x = *getNext()->value._int;
	}
	void get(unsigned* x)
	{
		*x = (unsigned)(*getNext()->value._int);
	}
	void get(short* x)
	{
		*x = (short)(*getNext()->value._int);
	}
	void get(float* x)
	{
		*x = *getNext()->value._float;
	}
	void get(Float2* x)
	{
		*x = *getNext()->value._float2;
	}

	void get(Float3* x)
	{
		*x = *getNext()->value._float3;
	}

	void get(Float4* x)
	{
		*x = *getNext()->value._float4;
	}

	void get(Float4x4* x)
	{
		*x = *getNext()->value._float4x4;
	}
	void get(std::string* x)
	{
		*x = *getNext()->value._string;
	}
	void get(IAttributePtr* x)
	{
		x->setIndex(*getNext()->value._int);
	}


	int get_Enum()
	{
		int x = *getNext()->value._int;
		return x;
	}
	
	void get_NotSupported()
	{
		// SKIP TO NEXT
		getNext();
	}

	bool hasNext()
	{
		if( index < (int)content.size())
			return true;
		else
		{
			reset();
			return false;
		}
	}

	DataItem* getNext()
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
		DataItem* nextItem = &content.at(index);
		index++;
		return nextItem;
	}
};
