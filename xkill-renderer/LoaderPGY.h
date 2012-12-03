#ifndef XKILL_RENDERER_LOADERPGY_H
#define XKILL_RENDERER_LOADERPGY_H

#include "Loader.h"
#include "MeshModel.h"
#include "SpecsPGY.h"

class LoaderPGY : public Loader
{
public:
	LoaderPGY(
		const std::string filePath, 
		const std::string fileName);
	~LoaderPGY();

	bool init();

	const MeshModel getMeshModel();
protected:
private:
	const MeshModel		loadPGY();
	const PGYHeader		loadHeader();
	const MeshMaterial	loadMaterial();

	MeshModel meshModel_;
};

#endif //XKILL_RENDERER_LOADERPGY_H