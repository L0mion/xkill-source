#ifndef XKILL_RENDERER_INSTANCEDDATA_H
#define XKILL_RENDERER_INSTANCEDDATA_H

typedef long HRESULT;

struct ID3D11Buffer;

struct VertexPosNormTexInstanced;

#include <vector>

static const unsigned int INSTANCED_DATA_START_MAX_INSTANCES = 5;

class InstancedData
{
public:
	InstancedData();
	~InstancedData();

	void addInstance(VertexPosNormTexInstanced instance);
	HRESULT update(ID3D11Device* device, ID3D11DeviceContext* devcon);
	void reset();

	unsigned int getInstanceCount() const;
	std::vector<VertexPosNormTexInstanced>& getInstanceVec();
	ID3D11Buffer* getInstanceBuffer() const;
protected:
private:
	HRESULT initBuffer(ID3D11Device* device);
	HRESULT increaseBufferCapacity(ID3D11Device* device, unsigned int target);
	HRESULT updateBuffer(ID3D11DeviceContext* devcon);
	void incrementMax(unsigned int target);

	unsigned int instanceCount_;
	unsigned int instanceMax_;
	std::vector<VertexPosNormTexInstanced> instanceVec_;

	ID3D11Buffer* instanceBuffer_;
};

#endif //XKILL_RENDERER_INSTANCEDDATA_H