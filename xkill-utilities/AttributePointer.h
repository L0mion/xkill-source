#pragma once

#include "dllUtilities.h"
// Extract the Object (variable name inside Parent) vector from Parent (class name) and cast it to Type (class name).
// Used when extracting attributes from inside another attribute.
// A useful and convenient macro that only occasionally scares little children.
#define ATTRIBUTE_CAST(Type,Object,Parent) &((std::vector<Type>*)Parent->Object.host)->at(Parent->Object.index)

/// Used to gain access to a specifc attribute in the attribute memory space
/** 
Stores a pointer to a std::vector with attributes of a specific type that needs
to be cast manually. A specific attribute can then be accessed through index.

Manual casting is necessary since DLLs does not support Templates.

\ingroup xkill-utilities
*/

//class DLL_U AttributePointer
//{
//public:
//	void* host;		//!< A void pointer to a std::vector holding attributes of a specific type.
//	int index;		//!< An index to an attribute in host.
//
//	AttributePointer();
//
//	void init(void* host, int index);
//};

#include <vector>

template <class T>
class AttributePtr
{
private:
	std::vector<T>* _hostArray;
public:
	int index;

	static void initClass(std::vector<T>* hostArray)
	{
		/*_hostArray = hostArray;
		_hostArray;*/
	}

	AttributePtr()
	{
		_hostArray = nullptr;
		index = -1;
	}

	bool isEmpty()
	{
		return _hostArray == nullptr;
	}

	void init(std::vector<T>* hostArray, int index)
	{
		_hostArray = hostArray;
		this->index = index;
	}

	T* getAttribute()
	{
		std::vector<T>* v = _hostArray;
		return &v->at(index);
	}
};

//template <class T>
//std::vector<T>* AttributePtr<T>::_hostArray = NULL;