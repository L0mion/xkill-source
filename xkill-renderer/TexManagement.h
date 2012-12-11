#ifndef XKILL_RENDERER_TEXMANAGEMENT_H
#define XKILL_RENDERER_TEXMANAGEMENT_H

typedef long HRESULT;

struct ID3D11Device;

class Tex;
class TexDesc;

#include <map>

class TexManagement
{
public:
	TexManagement();
	~TexManagement();

	HRESULT init();

	HRESULT handleTexDesc(
		TexDesc*		texDesc,
		ID3D11Device*	device);
	HRESULT createTex(
		unsigned int	texID, 
		std::string		texPath, 
		std::string		texFileName,
		ID3D11Device*	device);
protected:
private:
	std::vector<Tex*> texs_;
	std::map<
		unsigned int, 
		unsigned int> texIDtoIndex_;
};

#endif //XKILL_RENDERER_TEXMANAGEMENT_H