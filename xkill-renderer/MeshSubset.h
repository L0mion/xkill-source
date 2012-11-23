#ifndef XKILL_RENDERER_MESHSUBSET_H
#define XKILL_RENDERER_MESHSUBSET_H

class MeshSubset
{
public:
	MeshSubset(
		const unsigned int ssID,
		const unsigned int ssVertexStart,
		const unsigned int ssNumVertices,
		const unsigned int ssFaceStart,
		const unsigned int ssNumFaces);
	~MeshSubset();

	const unsigned int getSSID();
	const unsigned int getSSVertexStart();
	const unsigned int getSSNumVertices();
	const unsigned int getSSFaceStart();
	const unsigned int getSSNumFaces();
protected:
private:
	unsigned int ssID_;
	unsigned int ssVertexStart_;
	unsigned int ssNumVertices_;
	unsigned int ssFaceStart_;
	unsigned int ssNumFaces_;
};

#endif //XKILL_RENDERER_MESHSUBSET_H