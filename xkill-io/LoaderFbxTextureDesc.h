#ifndef XKILL_IO_LOADERFBXTEXTUREDESC_H
#define XKILL_IO_LOADERFBXTEXTUREDESC_H

#include <xkill-utilities/Math.h>

//! Class for storing texture data.
/*!
\ingroup xkill-io-fbx
*/
class LoaderFbxTextureDesc
{
public:
	//! Initializes LoaderFbxTextureDesc to its default state.
	LoaderFbxTextureDesc();
	//! Currently unused.
	~LoaderFbxTextureDesc();

	void setTextureName(const char* textureName);
	void setFileName(const char* fileName);
	void setScale(Float2 scale);
	void setTranslation(Float2 translation);
	void setRotation(Float3 rotation);
	void setSwap(bool swap);
	void setAlphaSource(int alphaSource);
	void setCroppingLeft(int croppingLeft);
	void setCroppingTop(int croppingTop);
	void setCroppingRight(int croppingRight);
	void setCroppingBottom(int croppingBottom);
	void setMappingType(int mappingType);
	void setPlanarMappingNormal(int planarMappingNormal);
	void setBlendMode(int blendMode);
	void setDefaultAlpha(float defaultAlpha);
	void setMaterialUse(int materialUse);
	void setTextureUse(int textureUse);

	const char* getTextureName();
	const char* getFileName();
	Float2		getScale();
	Float2		getTranslation();
	Float3		getRotation();
	bool		getSwap();
	int			getAlphaSource();
	int			getCroppingLeft();
	int			getCroppingTop();
	int			getCroppingRight();
	int			getCroppingBottom();
	int			getMappingType();
	int			getPlanarMappingNormal();
	int			getBlendMode();
	float		getDefaultAlpha();
	int			getMaterialUse();
	int			getTextureUse();
private:
	
	const char* textureName_;		//<! Name of the texture
	const char* fileName_;			//<! The textures file name.
									
	Float2	scale_;					//<! Texture scale.
	Float2	translation_;			//<! Texture translation.
	Float3	rotation_;				//<! Texture rotation.
	bool	swap_;					//<! Swap flag.
	int		alphaSource_;			//<! Source of alpha
	int		croppingLeft_;			//<! Left cropping.
	int		croppingTop_;			//<! Top cropping.
	int		croppingRight_;			//<! Right cropping.
	int		croppingBottom_;		//<! Bottom cropping.
	int		mappingType_;			//<! Type of mapping.
	int		planarMappingNormal_;	//<! Normal mapping planar.
	int		blendMode_;				//<! Blend mode.
	float	defaultAlpha_;			//<! Default alpha value.
	int		materialUse_;			//<! How materials are used.
	int		textureUse_;			//<! How texture is used.
};

#endif //XKILL_IO_LOADERFBXTEXTUREDESC_H