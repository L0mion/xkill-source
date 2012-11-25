#pragma once

#include "dllUtilities.h"
// Macro for accessing a attribute vector
// a useful and convinent macrot that only occasionally scares little children
#define ATTRIBUTE_CAST(Type,Object,Parent) &((std::vector<Type>*)Parent->Object.host)->at(Parent->Object.index)


/// Used to gain access to a specifc attribute in the attibute memory space
/** 
Stores a pointer to a std::vector with attributes of an specific type that needs
to be cast manually. A specific attribute can then be accessed through index.

Manual casting is neccissary since DLLs doesn't support Templates.

\ingroup xkill-utilities
*/

class DLL AttributePointer
{
public:
	
	
	void* host;		//!< A void pointer to a std::vector holding attributes of a specific type.
	int index;		//!< A index to a attribute in host

	AttributePointer()
	{
	}

	void init(void* host, int index)
	{
		this->host = host;
		this->index = index;
	}
};