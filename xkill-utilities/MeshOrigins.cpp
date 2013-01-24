#include "MeshOrigins.h"

MeshOrigins::MeshOrigins()
{
	//Get rid of this
}
MeshOrigins::MeshOrigins(std::string name)
{
	name_ = name;
}
MeshOrigins::~MeshOrigins()
{
	//Do nothing.
}

std::string MeshOrigins::getName()
{
	return name_;
}