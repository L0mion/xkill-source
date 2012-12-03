#ifndef XKILL_RENDERER_WRITERPGY_H
#define XKILL_RENDERER_WRITERPGY_H

#include <string>

#include "Writer.h"
#include "MeshModel.h"
#include "SpecsPGY.h"

static const float WRITER_PGY_VERSION = 0.1f;
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
	void writePGY();
	const PGYHeader loadHeader();
	void writeHeader(const PGYHeader header);
	void writeMaterials(const std::vector<MeshMaterial> materials);
	void writeMaterial(const MeshMaterial material);
	void writeGeometry(MeshGeometry geometry);
	void writeVertices(
		const unsigned int					numVertices, 
		const std::vector<VertexPosNormTex>	vertices);
	void writeVertex(const VertexPosNormTex vertex);
	void writeSubsets(
		const unsigned int				numSubsets, 
		const std::vector<MeshSubset>	subsets);
	void writeSubset(MeshSubset subset);
	void writeIndex(unsigned int index);

	MeshModel subject_;
};

#endif //XKILL_RENDERER_WRITERPGY_H