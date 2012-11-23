#pragma once

class IAttributeStorage;

class AttributeController
{
private:
	IAttributeStorage* host;
	int index;
public:
	AttributeController(IAttributeStorage* host, int index);

	void remove();
};