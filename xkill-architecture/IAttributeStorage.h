#pragma once

#include <vector>
#include <queue>

class IAttributeStorage
{
public:
	virtual void deleteAttribute(int index) = 0;
};
