#ifndef XKILL_IO_OBJ_H
#define XKILL_IO_OBJ_H

/*! \defgroup xkill-mesh-io-obj xkill-mesh-io-obj
	Loading of .obj-format.
	\ingroup xkill-mesh-io
*/

#include <vector>
#include <string>

#include "ObjGeometry.h"
#include "ObjDependencies.h"

struct VertexPosNormTex;

//! Representation of loaded .obj.
/*!
\ingroup xkill-mesh-io-obj
*/
class Obj
{
public:
	//! Does nothing.
	Obj();
	//! Initializes Obj with arguments passed.
	Obj(
		const std::string		filename,
		const ObjDependencies	objDependencies,
		const ObjGeometry		objGeometry);
	//! Does nothing.
	~Obj();

	const std::string getFileName();
	const std::vector<std::string> getMTLs();
	const ObjGeometry getObjGeometry();
protected:
private:
	std::string		fileName_;			//!< Original file-name of .obj.
	ObjDependencies	objDependencies_;	//!< Dependencies of geometry read from .obj. (this may include references to .mtl-files.)
	ObjGeometry		objGeometry_;		//!< Geometry of mesh read from .obj.
};

#endif //XKILL_IO_OBJ_H