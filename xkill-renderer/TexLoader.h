#ifndef XKILL_RENDERER_TEXLOADER_H
#define XKILL_RENDERER_TEXLOADER_H

typedef long HRESULT;

struct ID3D11Device;
struct ID3D11ShaderResourceView;

#include <string>

class TexLoader
{
public:
	TexLoader();
	~TexLoader();

	HRESULT createTexFromFile(
		ID3D11Device*				device,
		std::string					file,
		ID3D11ShaderResourceView**	srv);
protected:
private:
};

#endif //XKILL_RENDERER_TEXLOADER_H