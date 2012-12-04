#ifndef XKILL_RENDERER_OBJGROUP_H
#define XKILL_RENDERER_OBJGROUP_H

#include <vector>

//! Describes an .obj-group. (subset)
/*!
Describes part of a mesh that is supposed to be rendered with a corresponding material.
\ingroup xkill-mesh-io-obj
*/
class ObjGroup
{
public:
	//! Passes name of Group and sets material to default state.
	ObjGroup(const std::string groupName);
	//! Does nothing.
	~ObjGroup();

	//! Adds an index to group indices-vector.
	void pushIndex(unsigned int index);

	//! Sets material that belongs to group.
	void setMaterialName(std::string materialName);

	const std::string				getName();
	const std::string				getMaterial();
	const unsigned int				getNumIndices();
	const std::vector<unsigned int>	getIndices();
protected:
private:
	std::string					name_;			//!< Name of group.
	std::string					materialName_;	//!< Material the group is supposed to be rendered with.
	std::vector<unsigned int>	indices_;		//!< Indices making out group.
};

#endif //XKILL_RENDERER_OBJGROUP_H