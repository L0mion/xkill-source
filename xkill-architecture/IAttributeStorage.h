#pragma once

/// Interface to group AttributeStorage of different types togheter.
/** 
\ingroup achitecture
*/

class IAttributeStorage
{
public:
	virtual void deleteAttribute(int index) = 0;
};
