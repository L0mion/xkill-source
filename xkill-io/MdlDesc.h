#ifndef XKILL_IO_MDLDESC_H
#define XKILL_IO_MDLDESC_H

#include <string>
#include <vector>

#include "MdlDescHeader.h"
#include "MdlDescModel.h"

class MdlDesc
{
public:
	//! Initializes MdlDesc with passed variables.
	MdlDesc(
		std::string path,
		MdlDescHeader header,
		std::vector<MdlDescModel*> models);
	//! Does nothing.
	~MdlDesc();

	std::string getPath() const;
	MdlDescHeader getHeader() const;
	std::vector<MdlDescModel*> getModels() const;
protected:
private:
	std::string path_;						//!< Path to collection of textures.
	MdlDescHeader header_;					//!< Header of .mdldesc-file.
	std::vector<MdlDescModel*> models_;		//!< Collection of texture descriptions.
};

#endif //XKILL_IO_MDLDESC_H