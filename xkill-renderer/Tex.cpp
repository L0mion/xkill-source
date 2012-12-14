#include <d3d11.h>

#include "renderingUtilities.h"
#include "Tex.h"

Tex::Tex(
	const unsigned int id,
	ID3D11ShaderResourceView* srv)
{
	id_		= id;
	srv_	= srv;
}
Tex::~Tex()
{
	SAFE_RELEASE(srv_);
}

const unsigned int Tex::getID()			const
{
	return id_;
}
ID3D11ShaderResourceView* Tex::getSRV()	const
{
	return srv_;
}