#ifndef XKILL_RENDERER_WRITERPGY_H
#define XKILL_RENDERER_WRITERPGY_H

#include <string>

#include "Writer.h"
#include "MeshModel.h"

class WriterPGY : public Writer
{
public:
	WriterPGY(
		const MeshModel		subject,
		const std::string	filePath,
		const std::string	fileName);
	~WriterPGY();

	bool init();
protected:
private:
	void writeHeader();
	void writeMaterials(const std::vector<MeshMaterial> materials);

	MeshModel subject_;
};

#endif //XKILL_RENDERER_WRITERPGY_H