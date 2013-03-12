#include <iostream>

#include "LoaderMD5.h"
#include "LoaderMD5Model.h"
#include "LoaderMD5Animation.h"
#include "LoaderMD5Helper.h"

LoaderMD5::LoaderMD5()
{
	DirectX::XMStoreFloat4x4(&localToWorldMatrix_, DirectX::XMMatrixIdentity());
}
LoaderMD5::~LoaderMD5()
{
	
}

bool LoaderMD5::loadModel(const std::string& filename, LoaderMD5ModelDesc* modelDesc)
{
	LoaderMD5Model modelLoader;

	modelLoader.loadModel(filename, modelDesc);
	
	return true;
}

bool LoaderMD5::loadAnimation(const std::string& filename, LoaderMD5AnimationDesc* animationDesc)
{
	LoaderMD5Animation loaderAnimation;

	loaderAnimation.loadAnimation(filename, animationDesc);

	return true;
}