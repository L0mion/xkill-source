#pragma once

struct IAttribute;

/// Interface to group AttributeStorage of different types together.
/** 
\ingroup ARCHITECTURE
*/
class IAttributeStorage
{
public:
	virtual void deleteAttribute(int index) = 0;
	virtual IAttribute* getAttribute(int index) = 0;
};