#include "AttributeManager.h"

AttributeManager* AttributeManager::getInstance()
{
	static AttributeManager instance;
	return &instance;
}