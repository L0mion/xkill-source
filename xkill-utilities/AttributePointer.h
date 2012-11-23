#pragma once

#include <vector>

template <class T>
class AttributePointer
{
private:
	std::vector<T>* host;
	int index;
public:
	AttributePointer()
	{
	}

	void init(std::vector<T>* host, int index)
	{
		this->host = host;
		this->index = index;
	}

	T* getAttribute()
	{
		return &host->at(index);
	}
};