#ifndef XKILL_IO_MDLDESC_H
#define XKILL_IO_MDLDESC_H

#include <string>
#include <vector>

#include "MdlDescHeader.h"
#include "MdlDescModel.h"

class MdlDesc
{
public:
	MdlDesc(
		std::string path,
		MdlDescHeader header,
		std::vector<MdlDescModel*> models);
	~MdlDesc();

	std::string getPath() const;
	MdlDescHeader getHeader() const;
	std::vector<MdlDescModel*> getModels() const;
protected:
private:
	std::string path_;
	MdlDescHeader header_;
	std::vector<MdlDescModel*> models_;
};

#endif //XKILL_IO_MDLDESC_H