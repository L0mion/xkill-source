#include <stdio.h>
#include <vector>
#include <map>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxCommand.h>
#include <maya/MIOStream.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnMesh.h>

struct Float4
{
	float x,y,z,w;
	Float4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};
struct Float3
{
	float x,y,z;
	Float3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class exportLevel : public MPxCommand
{
    public:
        exportLevel();
        virtual ~exportLevel();
        MStatus doIt( const MArgList& );
        MStatus redoIt();
        MStatus undoIt();
        bool isUndoable() const;
        static void* creator();
};
exportLevel::exportLevel()
{
}
exportLevel::~exportLevel()
{
}

MStatus exportLevel::doIt( const MArgList& )
{
	/*std::vector<void> RBList;
	std::vector<void> MList;
	std::map<int,void> MIList;
	std::vector<void> Spawns;*/

    MString output;

	/*MSelectionList rigidbodies;
	MGlobal::displayInfo("*RigidBody*");
	MGlobal::selectByName( "*RigidBody*");
	MGlobal::getActiveSelectionList(rigidbodies);
	for( unsigned int i = 0; i < rigidbodies.length(); i++)
	{
		MObject obj;
		rigidbodies.getDependNode(i,obj);
		MFnDependencyNode fn(obj);
		output = fn.name().asChar();
		output = "\t" + output;
		MGlobal::displayInfo(output);
	}*/

	MGlobal::clearSelectionList();

	MSelectionList meshes;
	MGlobal::displayInfo("*Mesh*");
	MGlobal::selectByName( "*Mesh*");
	MGlobal::getActiveSelectionList(meshes);
	for( unsigned int i = 0; i < meshes.length(); i++)
	{
		MObject obj;+
		meshes.getDependNode(i,obj);
		MFnDependencyNode fn(obj);
		output = fn.name().asChar();
		output = "\t" + output + " type: " + obj.apiType();
		MGlobal::displayInfo(output);
		switch(obj.apiType())
		{
		case MFn::kTransform:
			// This is an instance of a mesh, output m to levelname.mdldesc
			break;
		case MFn::kMesh:
			// This is vertex data, export to obj
			break;
		}
	}
    return MS::kSuccess;


		// Handle scene lights
		//case MFn::kDirectionalLight:
		//	break;
		//case MFn::kPointLight:
		//	break;
		//case MFn::kSpotLight:
		//	break;
}
MStatus exportLevel::redoIt()
{
    return MS::kSuccess;
}
MStatus exportLevel::undoIt()
{
    return MS::kSuccess;
}
bool exportLevel::isUndoable() const
{
    return false;
}
void* exportLevel::creator()
{
    return new exportLevel();
}
MStatus initializePlugin( MObject obj )
{
    MFnPlugin plugin( obj, "XKill-team", "0.1", "Any" );
    plugin.registerCommand( "exportLevel", exportLevel::creator );
    return MS::kSuccess;
}
MStatus uninitializePlugin( MObject obj )
{
    MFnPlugin plugin( obj );
    plugin.deregisterCommand( "exportLevel" );
    return MS::kSuccess;
}