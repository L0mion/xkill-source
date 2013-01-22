#ifndef XKILL_RENDERER_INSTANCEDDATA_H
#define XKILL_RENDERER_INSTANCEDDATA_H

typedef long HRESULT;

struct ID3D11Buffer;

struct VertexPosNormTexInstanced;

#include <vector>

static const unsigned int INSTANCED_DATA_START_MAX_INSTANCES = 5; //Make me bigger in order to increase the starting size of instance-buffers.

class InstancedData
{
public:
	InstancedData();	//!< Initializes InstancedData to default state.
	~InstancedData();	//!< Releases buffer-object held by InstancedData.

	void addInstance(VertexPosNormTexInstanced instance);				//!< Adds an instance to InstancedData. Increments instanceCount.
	HRESULT update(ID3D11Device* device, ID3D11DeviceContext* devcon);	//!< Updates InstancedData and the buffer, increasing the size of it along the way if necessary.
	void reset();														//!< Sets instanceCount to 0 and clears vector with current instances.

	unsigned int getInstanceCount() const;
	std::vector<VertexPosNormTexInstanced>& getInstanceVec();
	ID3D11Buffer* getInstanceBuffer() const;
protected:
private:
	HRESULT initBuffer(ID3D11Device* device);									//!< Initializes buffer object.
	HRESULT increaseBufferCapacity(ID3D11Device* device, unsigned int target);	//!< Increases capacity of buffer-object.
	HRESULT updateBuffer(ID3D11DeviceContext* devcon);							//!< Maps new data to buffer-object.
	void incrementMax(unsigned int target);										//!< Doubles instanceMax until it is bigger or equal to target.

	unsigned int instanceCount_;							//!< Current number of instances held by InstancedData.
	unsigned int instanceMax_;								//!< Max instances helt by Instanceddata. (size of buffer)
	std::vector<VertexPosNormTexInstanced> instanceVec_;	//!< Current instances helt by InstancedData.

	ID3D11Buffer* instanceBuffer_;							//!< Instance vertex buffer.
};

#endif //XKILL_RENDERER_INSTANCEDDATA_H