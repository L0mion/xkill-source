#ifndef XKILL_IO_OBJDEPENDENCIES_H
#define XKILL_IO_OBJDEPENDENCIES_H

#include <vector>

//! Describes dependencies of a single parsed .obj-file.
/*!
References external .mtl-files, if the .obj-file utilizes materials.
\ingroup xkill-mesh-io-obj
*/
class ObjDependencies
{
public:
	//! Does nothing.
	ObjDependencies();
	//! Does nothing.
	~ObjDependencies();

	//! Pushes an .mtl-dependency to mtlFiles_-vector.
	void pushDependencyMTL(const std::string mtlFile);

	std::vector<std::string> getMTLFiles();
protected:
private:
	std::vector<std::string> mtlFiles_;	//!< Dependencies to .mtl-files referenced in .obj-file.
};

#endif //XKILL_IO_OBJDEPENDENCIES_H