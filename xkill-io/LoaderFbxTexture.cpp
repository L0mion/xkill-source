#include <fbxsdk.h>

#include <xkill-utilities/Math.h>

#include "LoaderFbxTexture.h"

LoaderFbxTexture::LoaderFbxTexture()
{
}
LoaderFbxTexture::~LoaderFbxTexture()
{
}
void LoaderFbxTexture::reset()
{
}

void LoaderFbxTexture::parseTexture(FbxGeometry* geometry)
{
	FbxProperty fbxProperty;
	if(geometry->GetNode() == NULL)
		return;  // Fix this shit!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	int numMaterials = geometry->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();

	for(int materialIndex=0; materialIndex<numMaterials; materialIndex++)
	{
		FbxSurfaceMaterial* surfaceMaterial = geometry->GetNode()->GetSrcObject<FbxSurfaceMaterial>(materialIndex);
		bool parseHeader = true;

		if(surfaceMaterial)
		{
			int textureIndex;
			FBXSDK_FOR_EACH_TEXTURE(textureIndex)
			{
				fbxProperty = surfaceMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[textureIndex]);
				parseTextureByProperty(&fbxProperty, parseHeader, materialIndex);
			}
		}
	}

}
void LoaderFbxTexture::parseTextureByProperty(FbxProperty* fbxProperty, bool& parseHeader, int materialIndex)
{
	if(fbxProperty->IsValid())
	{
		int textureCount = fbxProperty->GetSrcObjectCount<FbxTexture>();
		for(int textureIndex=0; textureIndex<textureCount; textureIndex++)
		{
			FbxLayeredTexture* layeredTexture = fbxProperty->GetSrcObject<FbxLayeredTexture>(textureIndex);
			if(layeredTexture)
			{
				parseTextureByPropertyLayered(fbxProperty, textureIndex);
			}
			else
			{
				parseTextureByPropertyNotLayered(fbxProperty, textureIndex);
			}
		}
	}
}
void LoaderFbxTexture::parseTextureByPropertyLayered(FbxProperty* fbxProperty, int textureIndex)
{
	FbxLayeredTexture* layeredTexture = fbxProperty->GetSrcObject<FbxLayeredTexture>(textureIndex);
	int numTextures = layeredTexture->GetSrcObjectCount<FbxTexture>();
	for(int i=0; i<numTextures; i++)
	{
		FbxTexture* texture = layeredTexture->GetSrcObject<FbxTexture>(i);
		if(texture)
		{
			FbxLayeredTexture::EBlendMode blendMode;
			layeredTexture->GetTextureBlendMode(i, blendMode);
			parseTextureInfo(texture, blendMode);
		}
	}
}
void LoaderFbxTexture::parseTextureByPropertyNotLayered(FbxProperty* fbxProperty, int textureIndex)
{
	FbxTexture* texture = fbxProperty->GetSrcObject<FbxTexture>(textureIndex);
	if(texture)
	{
		parseTextureInfo(texture, -1);
	}
}

void LoaderFbxTexture::parseTextureInfo(FbxTexture* texture, int blendMode)
{
	FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);
	FbxProceduralTexture* proceduralTexture = FbxCast<FbxProceduralTexture>(texture);

	const char* textureName;
	const char* fileName;
	
	Float2	scale				= Float2(0.0f, 0.0f);
	Float2	translation			= Float2(0.0f, 0.0f);
	Float3	rotation			= Float3(0.0f, 0.0f, 0.0f);
	bool	swap				= false;;
	int		alphaSource			= 0;
	int		croppingLeft		= 0;
	int		croppingTop			= 0;
	int		croppingRight		= 0;
	int		croppingBottom		= 0;
	int		mappingType			= 0;
	int		planarMappingNormal	= 0;
	float	defaultAlpha		= 0;
	int		materialUse			= 0;
	int		textureUse			= 0;

	textureName = texture->GetName();

	if(fileTexture)
		fileName = fileTexture->GetFileName();
	else if(proceduralTexture)
		fileName = "Procedural Texture";

	scale.x			= static_cast<float>(texture->GetScaleU());
	scale.y			= static_cast<float>(texture->GetScaleV());
	translation.x	= static_cast<float>(texture->GetTranslationU());
	translation.y	= static_cast<float>(texture->GetTranslationV());
	rotation.x		= static_cast<float>(texture->GetRotationU());
	rotation.y		= static_cast<float>(texture->GetRotationV());
	rotation.z		= static_cast<float>(texture->GetRotationW());
	swap			= texture->GetSwapUV();
	alphaSource		= texture->GetAlphaSource();
	croppingLeft	= texture->GetCroppingLeft();
	croppingTop		= texture->GetCroppingTop();
	croppingRight	= texture->GetCroppingRight();
	croppingBottom	= texture->GetCroppingBottom();
	mappingType		= texture->GetMappingType();

	if(mappingType == FbxTexture::ePlanar)
		planarMappingNormal = texture->GetPlanarMappingNormal();

	//Rember BlendMode!

	defaultAlpha = static_cast<float>(texture->GetDefaultAlpha());

	if(fileTexture)
		materialUse = fileTexture->GetMaterialUse();
	
	textureUse = texture->GetTextureUse();
}