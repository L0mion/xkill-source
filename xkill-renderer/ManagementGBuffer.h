#ifndef XKILL_RENDERER_MANAGEMENTGBUFFER_H
#define XKILL_RENDERER_MANAGEMENTGBUFFER_H

typedef long HRESULT;

class Winfo;
class GBuffer;
class ID3D11Device;

#include "gBufferID.h"

class ManagementGBuffer
{
public:
	ManagementGBuffer(Winfo* winfo);
	~ManagementGBuffer();

	void reset();
	HRESULT resize(ID3D11Device* device);
	HRESULT init(ID3D11Device* device);
protected:
private:
	HRESULT initAlbedo(ID3D11Device* device);
	HRESULT initNormal(ID3D11Device* device);
	HRESULT initMaterial(ID3D11Device* device);

	/*desc*/
	Winfo* winfo_;

	GBuffer* gBuffers_[GBUFFERID_NUM_BUFFERS];	//!< Containing data for deferred rendering.
};

#endif //XKILL_RENDERER_MANAGEMENTGBUFFER_H