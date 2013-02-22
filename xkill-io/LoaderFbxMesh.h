#ifndef XKILL_IO_LOADERFBXMESH_H
#define XKILL_IO_LOADERFBXMESH_H

#include <vector>
#include <DirectXMath.h>

#include <xkill-utilities/Math.h>

namespace fbxsdk_2014_0_beta2
{
	class FbxNode;
	class FbxMesh;
	class FbxVector2;
	class FbxVector4;

	class FbxLayerElementVertexColor;
	class FbxLayerElementNormal;
	class FbxLayerElementUV;
	class FbxLayerElementTangent;
	class FbxlayerElementBinormal;

	typedef FbxLayerElementVertexColor	FbxGeometryElementVertexColor;
	typedef FbxLayerElementNormal		FbxGeometryElementNormal;
	typedef FbxLayerElementUV			FbxGeometryElementUV;
	typedef FbxLayerElementTangent		FbxGeometryElementTangent;
	typedef FbxLayerElementBinormal		FbxGeometryElementBinormal;
}

class LoaderFbxMeshDesc;

//! Class for loading a FbxMesh
/*!
\ingroup xkill-io-fbx
*/
class LoaderFbxMesh
{
public:
	//!Initializes LoaderFbxMesh to its default state.
	LoaderFbxMesh();
	//! Releases all memory.
	~LoaderFbxMesh();
	//! Clears all vectors used to store data.
	void reset();

	//! Parses a FbxMesh object.
	/*!
	\param mesh A FbxMesh object.
	\param meshDesc Contains all information extracted from the FbxMesh.
	*/
	void parseMesh(FbxMesh* mesh, LoaderFbxMeshDesc* meshDesc);
private:
	//! Parses polygon groups in the mesh.
	/*!
	\param mesh FbxMesh object to be parsed.
	\param polygonIndex Index for a polygon inside the mesh.
	*/
	void parsePolygonGroup(FbxMesh* mesh, int polygonIndex);
	//! Retrieves the position for a vertex in the mesh.
	/*!
	\param mesh FbxMesh object to be parsed.
	\param controlPoints An array with control points.
	\param controlPointIndex An index to a controlPoint.
	*/
	void parseVertexPositions(FbxMesh* mesh, FbxVector4* controlPoints, int controlPointIndex);
	
	//! Retrieves the color for a vertex in the mesh.
	/*!
	\param mesh FbxMesh object to be parsed.
	\param controlPointIndex An index to a control point.
	\param vertexId An id for a vertex.
	*/
	void parseVertexColors(FbxMesh* mesh, int controlPointIndex, int vertexId);
	//! Retrieves the color for a vertex in the mesh by control point.
	/*!
	\param vertexColorElement Element containing color information.
	\param controlPointIndex An index to control point.
	*/
	FbxColor parseVertexColorsByControlPoint(FbxGeometryElementVertexColor* vertexColorElement, int controlPointIndex);
	//! Retrieves the color for a vertex in the mesh by polygon vertex.
	/*!
	\param vertexColorElement Element containing color information.
	\param controlPointIndex An id for a vertex.
	*/
	FbxColor parseVertexColorsByPolygonVertex(FbxGeometryElementVertexColor* vertexColorElement, int vertexId);
	
	//! Retrieves the normal for a vertex in the mesh.
	/*!
	\param mesh FbxMesh object to be parsed.
	\param controlPointIndex An index to a control point.
	\param vertexId An id for a vertex.
	*/
	void parseVertexNormals(FbxMesh* mesh, int controlPointIndex, int vertexId);
	//! Retrieves the normal for a vertex in the mesh by control point.
	/*!
	\param normalElement Element containing normal information.
	\param controlPointIndex An index to a control point.
	*/
	FbxVector4 parseVertexNormalsByControlPoint(FbxGeometryElementNormal* normalElement, int controlPointIndex);
	//! Retrieves the normal for a vertex in the mesh by polygon vertex.
	/*!
	\param normalElement Element containing normal information.
	\param controlPointIndex An id for a vertex
	*/
	FbxVector4 parseVertexNormalsByPolygonVertex(FbxGeometryElementNormal* normalElement, int vertexId);
	
	//! Retrieves the uv-coordinate for a vertex in the mesh.
	/*!
	\param mesh FbxMesh object to be parsed.
	\param polygonIndex An index to a polygon in the mesh.
	\param insidePolygonIndex An index to a vertex inside a polygon.
	\param controlPointIndex An index to a controlPoint.
	*/
	void parseVertexUVs(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex, int controlPointIndex);
	//! Retrieves the uv-coordinate for a vertex in the mesh by control point.
	/*!
	\param uvElement Element containing uv-coordinate information.
	\param controlPointIndex An index to a control point.
	*/
	FbxVector2 parseVertexUVsByControlPoint(FbxGeometryElementUV* uvElement, int controlPointIndex);
	//! Retrieves the uv-coordinate for a vertex in the mesh by polygon vertex.
	/*!
	\param mesh FbxMesh object to be parsed.
	\param uvElement Element containing uv-coordinate information.
	\param polygonIndex An index to a polygon in the mesh.
	\param insidePolygonIndex An index to a vertex inside a polygon.
	*/
	FbxVector2 parseVertexUVsByPolygonVertex(FbxMesh* mesh, FbxGeometryElementUV* uvElement, int polygonIndex, int insidePolygonIndex);
	
	//! Retrieves the tangent for a vertex in the mesh.
	/*!
	\param mesh FbxMesh object to be parsed.
	\param controlPointIndex An index to a control point.
	\param vertexId An id for a vertex.
	*/
	void parseVertexTangents(FbxMesh* mesh, int controlPointIndex, int vertexId);
	//! Retrieves the tangent for a vertex in the mesh by control point.
	/*!
	\param tangentElement Element containing tangent information.
	\param controlPointIndex An index to a control point.
	*/
	FbxVector4 parseVertexTangentsByControlPoint(FbxGeometryElementTangent* tangentElement, int controlPointIndex);
	//! Retrieves the tangent for a vertex in the mesh by polygon vertex.
	/*!
	\param tangentElement Element containing tangent information.
	\param vertexId An id for a vertex.
	*/
	FbxVector4 parseVertexTangentsByPolygonVertex(FbxGeometryElementTangent* tangentElement, int vertexId);

	//! Retrieves the binormal for a vertex in the mesh.
	/*!
	\param mesh FbxMesh object to be parsed.
	\param controlPointIndex An index to a control point.
	\param vertexId An id for a vertex.
	*/
	void parseVertexBinormals(FbxMesh* mesh, int controlPointIndex, int vertexId);
	//! Retrieves the binormal for a vertex in the mesh by control point.
	/*!
	\param binormalElement Element containing binormal information.
	\param controlPointIndex An index to a control point.
	*/
	FbxVector4 parseVertexBinormalsByControlPoint(FbxGeometryElementBinormal* binormalElement, int controlPointIndex);
	//! Retrieves the binormal for a vertex in the mesh by polygon vertex.
	/*!
	\param binormalElement Element containing binormal information.
	\param vertexId An id for a vertex.
	*/
	FbxVector4 parseVertexBinormalsByPolygonVertex(FbxGeometryElementBinormal* binormalElement, int vertexId);

	//! Parses link data that connects vertices to bones.
	/*!
	\param mesh The mesh to be parsed.
	\param meshDesc Mesh description that will store the results.
	*/
	void parseVertexLinkData(FbxMesh* mesh, LoaderFbxMeshDesc* meshDesc);
	//! Steps through the link hierarchy and index the link nodes and saves them in a vector.
	/*!
	\param rootNode Node at the root of the hierarchy.
	\param nodes Vector where the link nodes will be stored.
	\param parentIndices Vector where the link nodes parent indices will be stored.
	*/
	void parseLinkHierarchy(FbxNode* rootNode, std::vector<FbxNode*>* nodes, std::vector<int>* parentIndices);
	//! Retrives bone indices and weights for each vertex.
	/*!
	\param cluster A FbxCluster containing data regarding which vertex is effect by a link node.
	\param meshDesc Mesh description where results will be stored.
	\param nodeIndex Index to which node the cluster belongs to.
	*/
	void parseIndicesAndWeights(FbxCluster* cluster, LoaderFbxMeshDesc* meshDesc, int nodeIndex);
	//! Get the link nodes transform matrices.
	/*!
	\param cluster A FbxCluster containing the nodes matrix.
	\ param meshDesc Mesh description where the results will be stored.
	\param index Index of the bone the matrix belongs to.
	*/
	void parseTransformMatrix(FbxCluster* cluster, LoaderFbxMeshDesc* meshDesc, int index);
	
	//! Transforms the vertices into the local space.
	/*!
	\param mesh The mesh currently being parsed
	*/
	void transformVertices(FbxMesh* mesh);

	//! Finds the root node in a link hierarchy.
	/*!
	\param node A node somewhere in the hierarchy.
	*/
	FbxNode* findRoot(FbxNode* node);

	std::vector<int>	polygonGroupIds_; //!< Vector for storing polygon group ids.
	std::vector<Float3> vertexPositions_; //!< Vector for storing positions.
	std::vector<Float4> vertexColors_;	  //!< Vector for storing colors.
	std::vector<Float3> vertexNormals_;	  //!< Vector for storing normals.
	std::vector<Float2> vertexUVs_;		  //!< Vector for storing uv-coordinates.
	std::vector<Float4> vertexTangents_;  //!< Vector for storing tangents.
	std::vector<Float4> vertexBinormals_; //!< Vector for storing binormals.
};

#endif