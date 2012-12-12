#ifndef XKILL_IO_MDLDESCHEADER_H
#define XKILL_IO_MDLDESCHEADER_H

#include <string>

//! Describes fundemental data used to load several models into engine.
struct MdlDescHeader
{
	std::string name_;
	std::string path_;

	//! Does nothing.
	MdlDescHeader() {}
	//! Initializes MdlDescHeader.
	MdlDescHeader(
		std::string name,
		std::string path)
	{
		name_ = name;
		path_ = path;
	}
	//! Does nothing.
	~MdlDescHeader() {}
};

#endif //XKILL_IO_MDLDESCHEADER_H