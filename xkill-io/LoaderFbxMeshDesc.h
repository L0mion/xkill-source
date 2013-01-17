#ifndef XKILL_IO_LOADERFBXMESHDESC_H
#define XKILL_IO_LOADERFBXMESHDESC_H

#include <vector>

#include <xkill-utilities/Math.h>

class LoaderFbxMeshDesc
{
public:
	LoaderFbxMeshDesc();
	LoaderFbxMeshDesc(std::vector<int>	polygonGroupIds,
		std::vector<Float3> vertexPositions,
		std::vector<Float4> vertexColors,
		std::vector<Float3> vertexNormals,
		std::vector<Float2> vertexUVs,
		std::vector<Float4> vertexTangents,
		std::vector<Float4> vertexBinormals);
	~LoaderFbxMeshDesc();

	void setPolygonGroupIds(std::vector<int> polygonGroupIds);
	void setVertexPositions(std::vector<Float3> vertexPositions);
	void setVertexColors(std::vector<Float4> vertexColors);
	void setVertexNormals(std::vector<Float3> vertexNormals);
	void setVertexUVs(std::vector<Float2> vertexUVs);
	void setVertexTangents(std::vector<Float4> vertexTangents);
	void setVertexBinormals(std::vector<Float4> vertexBinormals);

	std::vector<int>	getPolygonGroupIds();	
	std::vector<Float3> getVertexPositions();
	std::vector<Float4> getVertexColors();
	std::vector<Float3>	getVertexNormals();
	std::vector<Float2>	getVertexUVs();
	std::vector<Float4>	getVertexTangents();
	std::vector<Float4>	getVertexBinormals();

private:
	std::vector<int>	polygonGroupIds_;
	std::vector<Float3> vertexPositions_;
	std::vector<Float4> vertexColors_;
	std::vector<Float3> vertexNormals_;
	std::vector<Float2> vertexUVs_;
	std::vector<Float4> vertexTangents_;
	std::vector<Float4> vertexBinormals_;
};

#endif //XKILL_IO_LOADERFBXMESHDESC_H