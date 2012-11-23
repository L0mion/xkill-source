#include "AttributeController.h"

AttributeController::AttributeController( IAttributeStorage* host, int index )
{
	this->host = host;
	this->index = index;
}

void AttributeController::remove()
{
	host->deleteAttribute(index);
}

