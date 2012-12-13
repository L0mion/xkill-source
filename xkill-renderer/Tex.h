#ifndef XKILL_RENDERER_TEX_H
#define XKILL_RENDERER_TEX_H

struct ID3D11ShaderResourceView;

//! Describes a loaded texture. Contains an ID to find correct texture, and a ID3D11ShaderResourceView to link to shader.
class Tex
{
public:
	//! Initializes Tex based on arguments.
	Tex(
		const unsigned int id,
		ID3D11ShaderResourceView* srv);
	//! Releases ID3D11ShaderResourceView-type object.
	~Tex();

	const unsigned int			getID()		const;
	ID3D11ShaderResourceView*	getSRV()	const;
protected:
private:
	unsigned int id_;				//!< ID of texture.
	ID3D11ShaderResourceView* srv_;	//!< Shader Resource used to bind texture to shader.
};

#endif //XKILL_RENDERER_TEX_H