#ifndef XKILL_RENDERER_LOADER_H
#define XKILL_RENDERER_LOADER_H

/*! \defgroup xkill-renderer-mesh xkill-renderer-mesh
	Group responsible for defining and loading meshes
	\ingroup xkill-renderer*/

#include <Windows.h>
#include <fstream>
#include <string>

//! Base class for a Mesh-loading component.
/*!
\ingroup xkill-renderer
*/
class Loader
{
public:
	//! Passes path of desired file to the loader.
	/*!
	\param mlFilePath Path to desired file.
	*/
	Loader(
		const std::string filePath,
		const std::string fileName);
	//! Closes ifstream if open.
	~Loader();

	//! Purely abstract functions to be used by all loaders to load respective files.
	virtual bool init()	= 0;
protected:
	const std::string getFilePath();
	const std::string getFileName();

	std::ifstream	ifstream_;	//!< ifstream used to load files.
private:
	std::string	filePath_;	//!< Path to desired file.
	std::string	fileName_;
};

#endif //XKILL_RENDERER_LOADER_H