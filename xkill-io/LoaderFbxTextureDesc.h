#ifndef XKILL_IO_LOADERFBXTEXTUREDESC_H
#define XKILL_IO_LOADERFBXTEXTUREDESC_H

#include <xkill-utilities/Math.h>

class LoaderFbxTextureDesc
{
public:
	LoaderFbxTextureDesc();
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
	
	const char* textureName_;
	const char* fileName_;
	
	Float2	scale_;
	Float2	translation_;
	Float3	rotation_;
	bool	swap_;
	int		alphaSource_;
	int		croppingLeft_;
	int		croppingTop_;
	int		croppingRight_;
	int		croppingBottom_;
	int		mappingType_;
	int		planarMappingNormal_;
	int		blendMode_;
	float	defaultAlpha_;
	int		materialUse_;
	int		textureUse_;
};

#endif //XKILL_IO_LOADERFBXTEXTUREDESC_H