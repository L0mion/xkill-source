#ifndef XKILL_RENDERER_MESHLOADER_H
#define XKILL_RENDERER_MESHLOADER_H

/*! \defgroup xkill-renderer-mesh xkill-renderer-mesh
	Group responsible for defining and loading meshes
	\ingroup xkill-renderer*/

#include <Windows.h>
#include <fstream>

//! Base class for a Mesh-loading component.
/*!
\ingroup xkill-renderer
*/
class MeshLoader
{
public:
	//! Passes path of desired file to the loader.
	/*!
	\param mlFilePath Path to desired file.
	*/
	MeshLoader(LPCWSTR mlFilePath);
	//! Closes ifstream if open.
	~MeshLoader();

	//! Purely abstract functions to be used by all loaders to load respective files.
	virtual bool init()	= 0;
protected:

	const LPCWSTR getMLFilePath();

	std::ifstream	mlIFS_;			//!< ifstream used to load files.
private:
	LPCWSTR			mlFilePath_;	//!< Path to desired file.
};

#endif //XKILL_RENDERER_MESHLOADER_H