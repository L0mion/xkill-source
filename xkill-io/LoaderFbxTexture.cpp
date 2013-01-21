#include <fbxsdk.h>

#include <xkill-utilities/Math.h>

#include "LoaderFbxTexture.h"
#include "LoaderFbxTextureDesc.h"

LoaderFbxTexture::LoaderFbxTexture()
{
}
LoaderFbxTexture::~LoaderFbxTexture()
{
}
void LoaderFbxTexture::reset()
{
}

void LoaderFbxTexture::parseTexture(FbxGeometry* geometry, LoaderFbxTextureDesc* textureDesc)
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
				parseTextureByProperty(&fbxProperty, parseHeader, materialIndex, textureDesc);
			}
		}
	}

}
void LoaderFbxTexture::parseTextureByProperty(FbxProperty* fbxProperty, bool& parseHeader, int materialIndex, LoaderFbxTextureDesc* textureDesc)
{
	if(fbxProperty->IsValid())
	{
		int textureCount = fbxProperty->GetSrcObjectCount<FbxTexture>();
		for(int textureIndex=0; textureIndex<textureCount; textureIndex++)
		{
			FbxLayeredTexture* layeredTexture = fbxProperty->GetSrcObject<FbxLayeredTexture>(textureIndex);
			if(layeredTexture)
			{
				parseTextureByPropertyLayered(fbxProperty, textureIndex, textureDesc);
			}
			else
			{
				parseTextureByPropertyNotLayered(fbxProperty, textureIndex, textureDesc);
			}
		}
	}
}
void LoaderFbxTexture::parseTextureByPropertyLayered(FbxProperty* fbxProperty, int textureIndex, LoaderFbxTextureDesc* textureDesc)
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
			parseTextureInfo(texture, blendMode, textureDesc);
		}
	}
}
void LoaderFbxTexture::parseTextureByPropertyNotLayered(FbxProperty* fbxProperty, int textureIndex, LoaderFbxTextureDesc* textureDesc)
{
	FbxTexture* texture = fbxProperty->GetSrcObject<FbxTexture>(textureIndex);
	if(texture)
	{
		parseTextureInfo(texture, -1, textureDesc);
	}
}

void LoaderFbxTexture::parseTextureInfo(FbxTexture* texture, int blendMode, LoaderFbxTextureDesc* textureDesc)
{
	FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);
	FbxProceduralTexture* proceduralTexture = FbxCast<FbxProceduralTexture>(texture);
	
	textureDesc->setTextureName(texture->GetName());

	if(fileTexture)
		textureDesc->setFileName(fileTexture->GetFileName());
	else if(proceduralTexture)
		textureDesc->setFileName("Unknown/Procedural Texture");

	textureDesc->setScale(Float2(static_cast<float>(texture->GetScaleU()), static_cast<float>(texture->GetScaleV())));
	textureDesc->setTranslation(Float2(static_cast<float>(texture->GetTranslationU()), static_cast<float>(texture->GetTranslationV())));
	textureDesc->setRotation(Float3(static_cast<float>(texture->GetRotationU()),
									static_cast<float>(texture->GetRotationV()),
									static_cast<float>(texture->GetRotationW())));
	textureDesc->setSwap(texture->GetSwapUV());
	textureDesc->setAlphaSource(texture->GetAlphaSource());

	textureDesc->setCroppingLeft(texture->GetCroppingLeft());
	textureDesc->setCroppingTop(texture->GetCroppingTop());
	textureDesc->setCroppingRight(texture->GetCroppingRight());
	textureDesc->setCroppingBottom(texture->GetCroppingBottom());
	textureDesc->setMappingType(texture->GetMappingType());

	if(textureDesc->getMappingType() == FbxTexture::ePlanar)
		textureDesc->setPlanarMappingNormal(texture->GetPlanarMappingNormal());

	textureDesc->setBlendMode(blendMode);
	textureDesc->setDefaultAlpha(static_cast<float>(texture->GetDefaultAlpha()));

	if(fileTexture)
		textureDesc->setMaterialUse(fileTexture->GetMaterialUse());
	
	textureDesc->setTextureUse(texture->GetTextureUse());
}