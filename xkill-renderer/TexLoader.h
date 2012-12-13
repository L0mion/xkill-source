#ifndef XKILL_RENDERER_TEXLOADER_H
#define XKILL_RENDERER_TEXLOADER_H

typedef long HRESULT;

struct ID3D11Device;
struct ID3D11ShaderResourceView;

#include <string>

//! Object used to load textures as shader resource views.
class TexLoader
{
public:
	//! Does nothing.
	TexLoader();
	//! Does nothing.
	~TexLoader();

	//! Creates an ID3D11ShaderResourceView-type object.
	/*!
	\param device	Handle to D3D-device.
	\param file		File to load.
	\param srv		inout-var to which the Shader Resource View will be loaded.
	\return HRESULT-type specifying any error which may have occured.
	*/
	HRESULT createTexFromFile(
		ID3D11Device*				device,
		std::string					file,
		ID3D11ShaderResourceView**	srv);
protected:
private:
};

#endif //XKILL_RENDERER_TEXLOADER_H