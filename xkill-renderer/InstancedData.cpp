#include "MeshVerticesInstanced.h"
#include "InstancedData.h"

InstancedData::InstancedData()
{
	meshID_			= 0;
	instanceCount_	= 0;
}
InstancedData::InstancedData(unsigned int meshID)
{
	meshID_			= meshID;

	instanceCount_	= 0;
}
InstancedData::~InstancedData()
{
	//Do nothing.
}

void InstancedData::addInstance(VertexPosNormTexInstanced instance)
{
	if(instanceCount_ < instanceVec_.capacity())
	{
		instanceVec_[instanceCount_] = instance;
	}
	else
	{
		instanceVec_.push_back(instance);
	}

	instanceCount_++;
}
void InstancedData::reset()
{
	instanceCount_ = 0;
	instanceVec_.clear();
}

unsigned int InstancedData::getInstanceCount() const
{
	return instanceCount_;
}
std::vector<VertexPosNormTexInstanced>& InstancedData::getInstanceVec()
{
	return instanceVec_;
}