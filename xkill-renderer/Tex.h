#ifndef XKILL_RENDERER_TEX_H
#define XKILL_RENDERER_TEX_H

struct ID3D11ShaderResourceView;

class Tex
{
public:
	Tex(
		const unsigned int id,
		ID3D11ShaderResourceView* srv);
	~Tex();

	const unsigned int			getID()		const;
	ID3D11ShaderResourceView*	getSRV()	const;
protected:
private:
	unsigned int id_;
	ID3D11ShaderResourceView* srv_;
};

#endif //XKILL_RENDERER_TEX_H