#ifndef XKILL_RENDERER_INSTANCEDDATA_H
#define XKILL_RENDERER_INSTANCEDDATA_H

#include <vector>

struct VertexPosNormTexInstanced;

class InstancedData
{
public:
	InstancedData();
	InstancedData(unsigned int meshID);
	~InstancedData();

	void addInstance(VertexPosNormTexInstanced instance);
	void reset();

	unsigned int getInstanceCount() const;
	std::vector<VertexPosNormTexInstanced>& getInstanceVec();
protected:
private:
	unsigned int meshID_;
	unsigned int instanceCount_;
	std::vector<VertexPosNormTexInstanced> instanceVec_;
};

#endif //XKILL_RENDERER_INSTANCEDDATA_H