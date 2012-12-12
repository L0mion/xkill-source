#ifndef XKILL_RENDERER_TEXMANAGEMENT_H
#define XKILL_RENDERER_TEXMANAGEMENT_H

typedef long HRESULT;

struct ID3D11Device;
struct ID3D11ShaderResourceView;

class Tex;
class TexDesc;
class TexLoader;

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

	ID3D11ShaderResourceView* getTexSrv(const unsigned int texID) const;
	int getTexsIndex(const unsigned int texID) const;
protected:
private:
	HRESULT createTex(
		unsigned int	texID, 
		std::string		texPath, 
		std::string		texFileName,
		ID3D11Device*	device);
	void pushTex(
		unsigned int				texID,
		ID3D11ShaderResourceView*	srv);

	/*intermediate*/
	TexLoader* texLoader_;

	/*result*/
	std::vector<Tex*> texs_;
	std::map<
		unsigned int, 
		unsigned int> texIDtoIndex_;
};

#endif //XKILL_RENDERER_TEXMANAGEMENT_H