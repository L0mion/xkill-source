#ifndef XKILL_IO_MDLDESCHEADER_H
#define XKILL_IO_MDLDESCHEADER_H

#include <string>

struct MdlDescHeader
{
	std::string name_;
	std::string path_;

	MdlDescHeader() {}
	MdlDescHeader(
		std::string name,
		std::string path)
	{
		name_ = name;
		path_ = path;
	}
	~MdlDescHeader() {}
};

#endif //XKILL_IO_MDLDESCHEADER_H