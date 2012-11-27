#include <MeshSubset.h>

MeshSubset::MeshSubset(
	const unsigned int ssID,
	const unsigned int ssVertexStart,
	const unsigned int ssNumVertices,
	const unsigned int ssFaceStart,
	const unsigned int ssNumFaces)
{
	ssID_			= ssID;
	ssVertexStart_	= ssVertexStart;
	ssNumVertices_	= ssNumVertices;
	ssFaceStart_	= ssFaceStart;
	ssNumFaces_		= ssNumFaces;
}
MeshSubset::~MeshSubset()
{
	//Do nothing.
}

const unsigned int MeshSubset::getSSID()
{
	return ssID_;
}
const unsigned int MeshSubset::getSSVertexStart()
{
	return ssVertexStart_;
}
const unsigned int MeshSubset::getSSNumVertices()
{
	return ssNumVertices_;
}
const unsigned int MeshSubset::getSSFaceStart()
{
	return ssFaceStart_;
}
const unsigned int MeshSubset::getSSNumFaces()
{
	return ssNumFaces_;
}